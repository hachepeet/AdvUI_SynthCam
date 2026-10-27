#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_video/juce_video.h>

// Inspired by https://github.com/juce-framework/JUCE/blob/29396c22c93392d6738e021b83196283d6e4d850/examples/GUI/CameraDemo.h

class CameraView final : public juce::Component
{
public:
    CameraView();
    ~CameraView() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    void updateCameraList();
    void cameraChanged();
    void openCamera();
    void cameraDeviceOpenResult(juce::CameraDevice *, const juce::String &);
    void takeSnapshot();
    void imageReceived(const juce::Image &);

    std::unique_ptr<juce::CameraDevice> cameraDevice;
    std::unique_ptr<juce::Component> cameraPreview;
    juce::ImageComponent lastSnapshot;
    juce::ComboBox cameraSelector{"Camera"};
    juce::TextButton snapshotButton{"Take snapshot"};
    juce::String statusText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CameraView)
};
