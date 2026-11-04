#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Shared/Camera.h"
#include <functional>

class FXView final : public juce::Component
{
public:
    FXView();

    void paint(juce::Graphics &) override;
    void resized() override;

    juce::Slider &getDelaySlider() noexcept;
    juce::Slider &getDryWetSlider() noexcept;
    juce::Slider &getRoomSizeSlider() noexcept;
    juce::Slider &getReverbDryWetSlider() noexcept;

    juce::ComboBox &getEffectSelector() noexcept;
    void setHandPositionCallback(std::function<void(float, float, bool, bool)> callback);

private:
    juce::Label pluginTitle;
    Camera camera;
    juce::Slider delaySlider;
    juce::Slider dryWetSlider;
    juce::Slider roomSizeSlider;
    juce::Slider reverbDryWetSlider;
    juce::ComboBox effectSelector;
    bool delaySelected;

    juce::Label delayMinLabel;
    juce::Label delayMaxLabel;
    juce::Label delayAxisLabel;
    juce::Label dryWetMinLabel;
    juce::Label dryWetMaxLabel;
    juce::Label dryWetAxisLabel;

    juce::Label roomMinLabel;
    juce::Label roomMaxLabel;
    juce::Label roomAxisLabel;
    juce::Label revDWMinLabel;
    juce::Label revDWMaxLabel;
    juce::Label revDWAxisLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXView)
};
