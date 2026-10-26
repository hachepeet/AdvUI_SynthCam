#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

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

private:
    // juce::Slider frequencySlider;
    // juce::Slider volumeSlider;
    juce::Slider delaySlider;
    juce::Slider dryWetSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXView)
};
 