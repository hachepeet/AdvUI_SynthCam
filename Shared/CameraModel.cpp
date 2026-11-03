#include "CameraModel.h"

CameraModel::~CameraModel()
{
    stopHandTracker();
}

bool CameraModel::startHandTracker()
{
    stopHandTracker();

    handTrackerDirectory = juce::File::getSpecialLocation(juce::File::tempDirectory)
                               .getChildFile("synthcam-hand-tracking");
    handTrackerDirectory.createDirectory();
    handTrackerDirectory.getChildFile("status.txt").deleteFile();
    receivedFrameCount = 0;
    writtenFrameCount = 0;
    failedFrameCount = 0;
    frameSequence = 0;

    handTracker = std::make_unique<juce::ChildProcess>();
    juce::StringArray arguments{"python", "-u", SYNTHCAM_HAND_TRACKER_SCRIPT_PATH,
                                handTrackerDirectory.getFullPathName()};
    if (!handTracker->start(arguments, juce::ChildProcess::wantStdErr))
    {
        handTracker.reset();
        return false;
    }

    return true;
}

void CameraModel::stopHandTracker()
{
    if (handTracker != nullptr)
    {
        if (handTracker->isRunning())
            handTracker->kill();
        handTracker.reset();
    }

    if (handTrackerDirectory.isDirectory())
        handTrackerDirectory.deleteRecursively();
    handTrackerDirectory = juce::File();
}

void CameraModel::submitFrame(const juce::Image &image)
{
    ++receivedFrameCount;
    if (handTracker == nullptr || !handTracker->isRunning() || !image.isValid())
        return;

    const auto sequence = ++frameSequence;
    const auto frame = handTrackerDirectory.getChildFile("frame_" + juce::String(sequence) + ".jpg");
    const auto temporaryFrame = handTrackerDirectory.getChildFile("frame_" + juce::String(sequence) + ".tmp");
    bool written = false;
    {
        std::unique_ptr<juce::FileOutputStream> output(temporaryFrame.createOutputStream());
        if (output != nullptr)
        {
            juce::JPEGImageFormat jpeg;
            written = jpeg.writeImageToStream(image, *output);
            output->flush();
        }
    }

    if (written && temporaryFrame.moveFileTo(frame))
        ++writtenFrameCount;
    else
    {
        temporaryFrame.deleteFile();
        frame.deleteFile();
        ++failedFrameCount;
    }
}

CameraModel::HandState CameraModel::pollHandState()
{
    HandState state;
    state.receivedFrames = receivedFrameCount.load();
    state.writtenFrames = writtenFrameCount.load();
    state.failedFrames = failedFrameCount.load();

    if (handTracker == nullptr)
    {
        state.status = HandState::Status::unavailable;
        return state;
    }

    const auto statusFile = handTrackerDirectory.getChildFile("status.txt");
    if (statusFile.existsAsFile())
    {
        const auto status = statusFile.loadFileAsString().trim();
        if (status.isNotEmpty() && status != "ready")
        {
            state.status = HandState::Status::unavailable;
            state.diagnostic = status;
            return state;
        }
    }

    if (!handTracker->isRunning())
    {
        state.status = HandState::Status::stopped;
        return state;
    }

    juce::Array<juce::File> positionFiles;
    handTrackerDirectory.findChildFiles(positionFiles, juce::File::findFiles,
                                        false, "right_hand_*.txt");
    if (positionFiles.isEmpty())
    {
        state.status = HandState::Status::waiting;
        return state;
    }

    auto positionFile = positionFiles.getFirst();
    for (const auto &candidate : positionFiles)
        if (candidate.getFileName() > positionFile.getFileName())
            positionFile = candidate;

    const auto position = positionFile.loadFileAsString().trim();
    if (position == "none")
    {
        state.status = HandState::Status::notDetected;
        state.diagnostic = handTrackerDirectory.getChildFile("diagnostics.txt")
                               .loadFileAsString()
                               .trim();
        return state;
    }

    const auto values = juce::StringArray::fromTokens(position, ",", "");
    if (values.size() != 3)
    {
        state.status = HandState::Status::waiting;
        return state;
    }

    const auto x = values[0].getDoubleValue();
    const auto y = values[1].getDoubleValue();
    state.position = {static_cast<float>(1.0 - juce::jlimit(0.0, 1.0, x)),
                      static_cast<float>(juce::jlimit(0.0, 1.0, y))};
    state.isClosed = values[2].getIntValue() != 0;
    state.status = HandState::Status::detected;
    return state;
}
