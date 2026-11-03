#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Shared/Camera.h"
#include <functional>

class SynthView final : public juce::Component
{
public:
    SynthView();

    void paint(juce::Graphics &) override;
    void resized() override;

    juce::Slider &getFrequencySlider() noexcept;
    juce::Slider &getWaveformSlider() noexcept;
    void setHandPositionCallback(std::function<void(float, float, bool, bool)> callback);

private:
    Camera camera;
    juce::Slider frequencySlider;
    juce::Slider waveformSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthView)
};
