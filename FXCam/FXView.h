#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "CameraView.h"

class FXView final : public juce::Component
{
public:
    FXView();

    void paint(juce::Graphics &) override;
    void resized() override;

    // juce::Slider &getFrequencySlider() noexcept;
    // juce::Slider &getVolumeSlider() noexcept;
    juce::Slider &getDelaySlider() noexcept;
    juce::Slider &getDryWetSlider() noexcept;
    juce::Slider &getRoomSizeSlider() noexcept;
    juce::Slider &getReverbDryWetSlider() noexcept;

private:
    CameraView cameraView;
    juce::Slider delaySlider;
    juce::Slider dryWetSlider;
    juce::Slider roomSizeSlider;
    juce::Slider reverbDryWetSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXView)
};
