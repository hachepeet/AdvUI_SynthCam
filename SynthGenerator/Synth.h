#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Shared/Camera.h"
#include <functional>

class SynthView;

class Synth final : public juce::Component
{
public:
    Synth();
    ~Synth() override;

    juce::Slider &getFrequencySlider() noexcept;
    juce::Slider &getWaveformSlider() noexcept;
    void setHandPositionCallback(std::function<void(float, float, bool, bool)> callback);

    void resized() override;

private:
    std::unique_ptr<SynthView> view;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Synth)
};
