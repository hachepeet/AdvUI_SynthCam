// Declares the shared camera model.
#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <atomic>

class CameraModel final
{
public:
    struct HandState
    {
        enum class Status
        {
            unavailable,
            stopped,
            waiting,
            notDetected,
            detected
        };

        Status status = Status::waiting;
        juce::String diagnostic;
        juce::Point<float> position;
        bool isClosed = false;
        int receivedFrames = 0;
        int writtenFrames = 0;
        int failedFrames = 0;
    };

    CameraModel() = default;
    ~CameraModel();

    bool startHandTracker();
    void stopHandTracker();
    void submitFrame(const juce::Image &image);
    HandState pollHandState();

private:
    std::unique_ptr<juce::ChildProcess> handTracker;
    juce::File handTrackerDirectory;
    std::atomic<int> receivedFrameCount{0};
    std::atomic<int> writtenFrameCount{0};
    std::atomic<int> failedFrameCount{0};
    std::atomic<unsigned int> frameSequence{0};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CameraModel)
};
