#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_video/juce_video.h>
#include <atomic>
#include <functional>

// Inspired by https://github.com/juce-framework/JUCE/blob/29396c22c93392d6738e021b83196283d6e4d850/examples/GUI/CameraDemo.h

class CameraView final : public juce::Component,
                         private juce::Timer,
                         private juce::CameraDevice::Listener
{
public:
    CameraView();
    ~CameraView() override;

    void setHandPositionCallback(std::function<void(float, float)> callback);

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    void timerCallback() override;
    void imageReceived(const juce::Image &) override;
    void updateCameraList();
    void cameraChanged();
    void openCamera();
    void cameraDeviceOpenResult(juce::CameraDevice *, const juce::String &);
    void takeSnapshot();
    void snapshotReceived(const juce::Image &);
    void startHandTracker();
    void stopHandTracker();
    void submitFrameToHandTracker(const juce::Image &);
    void updateHandPosition();

    std::unique_ptr<juce::CameraDevice> cameraDevice;
    std::unique_ptr<juce::Component> cameraPreview;
    std::unique_ptr<juce::ChildProcess> handTracker;
    juce::File handTrackerDirectory;
    juce::ImageComponent lastSnapshot;
    juce::ComboBox cameraSelector{"Camera"};
    juce::TextButton snapshotButton{"Take snapshot"};
    juce::Label handPositionLabel;
    std::function<void(float, float)> handPositionCallback;
    juce::String statusText;
    std::atomic<int> receivedFrameCount{0};
    std::atomic<int> writtenFrameCount{0};
    std::atomic<int> failedFrameCount{0};
    std::atomic<unsigned int> frameSequence{0};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CameraView)
};
