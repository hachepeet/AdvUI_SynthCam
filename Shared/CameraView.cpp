#include "CameraView.h"

#include "Theme.h"

// Inspired by https://github.com/juce-framework/JUCE/blob/29396c22c93392d6738e021b83196283d6e4d850/examples/GUI/CameraDemo.h

CameraView::CameraView()
{
    addAndMakeVisible(cameraSelector);
    addAndMakeVisible(snapshotButton);
    addAndMakeVisible(lastSnapshot);
    addAndMakeVisible(handPositionLabel);
    lastSnapshot.setVisible(false);
    handPositionLabel.setJustificationType(juce::Justification::centredLeft);
    handPositionLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    handPositionLabel.setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.75f));
    handPositionLabel.setText("Right hand: not detected", juce::dontSendNotification);

    snapshotButton.setEnabled(false);
    snapshotButton.onClick = [this]
    { takeSnapshot(); };
    cameraSelector.onChange = [this]
    { cameraChanged(); };

    updateCameraList();
    cameraSelector.setSelectedId(1);

    const auto cameras = juce::CameraDevice::getAvailableDevices();
    if (cameras.size() > 0)
    {
        cameraSelector.setSelectedId(2);
        statusText = "Opening camera...";
        openCamera();
    }
    else
    {
        statusText = "No camera available";
    }
}

CameraView::~CameraView()
{
    stopTimer();
    if (cameraDevice != nullptr)
        cameraDevice->removeListener(this);
    stopHandTracker();
    cameraPreview.reset();
    cameraDevice.reset();
}

void CameraView::setHandPositionCallback(std::function<void(float, float)> callback)
{
    handPositionCallback = std::move(callback);
}

void CameraView::paint(juce::Graphics &graphics)
{
    graphics.fillAll(SynthCamTheme::backgroundColour);

    auto previewArea = getLocalBounds().withTrimmedTop(32);
    graphics.setColour(juce::Colours::black);
    graphics.fillRect(previewArea);

    graphics.setColour(juce::Colours::white);
    graphics.setFont(juce::FontOptions(16.0f));
    if (cameraPreview == nullptr)
        graphics.drawFittedText(statusText, previewArea.reduced(12),
                                juce::Justification::centred, 2);
}

void CameraView::resized()
{
    auto controls = getLocalBounds().removeFromTop(28);
    cameraSelector.setBounds(controls.removeFromLeft(180));
    controls.removeFromLeft(8);
    snapshotButton.setBounds(controls.removeFromLeft(120));

    auto previewArea = getLocalBounds().reduced(1);
    previewArea.removeFromTop(31);
    if (cameraPreview != nullptr)
    {
        cameraPreview->setBounds(previewArea);
        cameraPreview->setTransform(juce::AffineTransform::scale(
            -1.0f, 1.0f,
            cameraPreview->getWidth() * 0.5f,
            cameraPreview->getHeight() * 0.5f));
    }

    lastSnapshot.setBounds(previewArea);
    lastSnapshot.setTransform(juce::AffineTransform::scale(
        -1.0f, 1.0f,
        lastSnapshot.getWidth() * 0.5f,
        lastSnapshot.getHeight() * 0.5f));
    handPositionLabel.setBounds(previewArea.removeFromTop(28).reduced(4, 2));
}

void CameraView::updateCameraList()
{
    cameraSelector.clear();
    cameraSelector.addItem("No camera", 1);
    cameraSelector.addSeparator();

    const auto cameras = juce::CameraDevice::getAvailableDevices();
    for (int i = 0; i < cameras.size(); ++i)
        cameraSelector.addItem(cameras[i], i + 2);
}

void CameraView::cameraChanged()
{
    stopTimer();
    if (cameraDevice != nullptr)
        cameraDevice->removeListener(this);
    stopHandTracker();
    cameraPreview.reset();
    cameraDevice.reset();
    lastSnapshot.setVisible(false);
    snapshotButton.setEnabled(false);
    handPositionLabel.setText("Right hand: not detected", juce::dontSendNotification);
    statusText = "No camera selected";

    if (cameraSelector.getSelectedId() > 1)
        openCamera();

    resized();
    repaint();
}

void CameraView::openCamera()
{
    const auto selectedIndex = cameraSelector.getSelectedId() - 2;
    if (selectedIndex < 0)
        return;

#if JUCE_ANDROID || JUCE_IOS
    juce::Component::SafePointer<CameraView> safeThis(this);
    juce::CameraDevice::openDeviceAsync(selectedIndex,
                                        [safeThis](juce::CameraDevice *device,
                                                   const juce::String &error)
                                        {
                                            if (safeThis != nullptr)
                                                safeThis->cameraDeviceOpenResult(device, error);
                                        });
#else
    cameraDeviceOpenResult(juce::CameraDevice::openDevice(selectedIndex), {});
#endif
}

void CameraView::cameraDeviceOpenResult(juce::CameraDevice *device,
                                        const juce::String &error)
{
    cameraDevice.reset(device);

    if (cameraDevice == nullptr)
    {
        statusText = error.isNotEmpty() ? "Camera open failed: " + error
                                        : "Camera open failed";
        repaint();
        return;
    }

    cameraPreview.reset(cameraDevice->createViewerComponent());
    addAndMakeVisible(*cameraPreview);
    cameraDevice->addListener(this);
    handPositionLabel.toFront(false);
    snapshotButton.setEnabled(true);
    startHandTracker();
    statusText.clear();
    resized();
    repaint();
}

void CameraView::takeSnapshot()
{
    if (cameraDevice == nullptr)
        return;

    juce::Component::SafePointer<CameraView> safeThis(this);
    cameraDevice->takeStillPicture(
        [safeThis](const juce::Image &image)
        {
            if (safeThis != nullptr)
                safeThis->snapshotReceived(image);
        });
}

void CameraView::snapshotReceived(const juce::Image &image)
{
    if (image.isValid())
    {
        lastSnapshot.setImage(image);
        lastSnapshot.setVisible(true);
        lastSnapshot.toFront(false);
        handPositionLabel.toFront(false);
    }
}

void CameraView::timerCallback()
{
    updateHandPosition();
}

void CameraView::imageReceived(const juce::Image &image)
{
    ++receivedFrameCount;
    submitFrameToHandTracker(image);
}

// Frames management was written with the help of GitHub Copilot
void CameraView::startHandTracker()
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
        handPositionLabel.setText("Right hand: tracker unavailable", juce::dontSendNotification);
        return;
    }

    startTimer(100);
}

void CameraView::stopHandTracker()
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

void CameraView::submitFrameToHandTracker(const juce::Image &image)
{
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
    {
        ++writtenFrameCount;
    }
    else
    {
        temporaryFrame.deleteFile();
        frame.deleteFile();
        ++failedFrameCount;
    }
}

// Debugging and diagnostics were written with the help of GitHub Copilot
void CameraView::updateHandPosition()
{
    if (handTracker == nullptr)
        return;

    const auto statusFile = handTrackerDirectory.getChildFile("status.txt");
    if (statusFile.existsAsFile())
    {
        const auto status = statusFile.loadFileAsString().trim();
        if (status.isNotEmpty() && status != "ready")
        {
            handPositionLabel.setText(status, juce::dontSendNotification);
            return;
        }
    }

    if (!handTracker->isRunning())
    {
        handPositionLabel.setText("Right hand: tracker stopped", juce::dontSendNotification);
        return;
    }

    juce::Array<juce::File> positionFiles;
    handTrackerDirectory.findChildFiles(positionFiles, juce::File::findFiles,
                                        false, "right_hand_*.txt");
    if (positionFiles.isEmpty())
    {
        handPositionLabel.setText("Right hand: waiting (callbacks " + juce::String(receivedFrameCount.load()) + ", writes " + juce::String(writtenFrameCount.load()) + ", failures " + juce::String(failedFrameCount.load()) + ")",
                                  juce::dontSendNotification);
        return;
    }

    auto positionFile = positionFiles.getFirst();
    for (const auto &candidate : positionFiles)
        if (candidate.getFileName() > positionFile.getFileName())
            positionFile = candidate;
    const auto position = positionFile.loadFileAsString().trim();
    if (position == "none")
    {
        auto diagnostic = handTrackerDirectory.getChildFile("diagnostics.txt")
                              .loadFileAsString()
                              .trim();
        handPositionLabel.setText("Right hand: not detected" + (diagnostic.isNotEmpty() ? " (" + diagnostic + ")" : ""),
                                  juce::dontSendNotification);
        return;
    }

    const auto values = juce::StringArray::fromTokens(position, ",", "");
    if (values.size() != 2)
        return;

    const auto x = values[0].getDoubleValue();
    const auto y = values[1].getDoubleValue();
    handPositionLabel.setText("Right hand: X " + juce::String(x, 2) + "  Y " + juce::String(y, 2),
                              juce::dontSendNotification);

    if (handPositionCallback != nullptr)
    {
        const auto flippedX = 1.0 - juce::jlimit(0.0, 1.0, x);
        const auto flippedY = 1.0 - juce::jlimit(0.0, 1.0, y);
        handPositionCallback(static_cast<float>(flippedX),
                             static_cast<float>(flippedY));
    }
}
