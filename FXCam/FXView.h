#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "CameraView.h"

class FXView final : public juce::Component
{
public:
    FXView();

    void paint(juce::Graphics &) override;
    void resized() override;

    juce::Slider &getFrequencySlider() noexcept;
    juce::Slider &getVolumeSlider() noexcept;

private:
    CameraView cameraView;
    juce::Slider frequencySlider;
    juce::Slider volumeSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXView)
};
