#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Shared/CameraView.h"
#include <functional>

class SynthView final : public juce::Component
{
public:
    SynthView();

    void paint(juce::Graphics &) override;
    void resized() override;

    juce::Slider &getFrequencySlider() noexcept;
    juce::Slider &getWaveformSlider() noexcept;
    void setHandPositionCallback(std::function<void(float, float)> callback);

private:
    CameraView cameraView;
    juce::Slider frequencySlider;
    juce::Slider waveformSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthView)
};
