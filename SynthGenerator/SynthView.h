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
    juce::Label pluginTitle;
    Camera camera;
    juce::Slider frequencySlider;
    juce::Slider waveformSlider;

    juce::Label sinLabel;
    juce::Label sqLabel;
    juce::Label triLabel;
    juce::Label rampLabel;
    juce::Label sawLabel;
    juce::Label waveAxisLabel;

    juce::Label fqMinLabel;
    juce::Label fqMaxLabel;
    juce::Label fqDefaultLabel;
    juce::Label fqAxisLabel; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthView)
};
