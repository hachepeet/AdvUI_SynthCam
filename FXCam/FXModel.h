#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Delay.h"
#include "ReverbFX.h"

class FXModel final
{
public:
    explicit FXModel(juce::AudioProcessorValueTreeState &parameters);

    void prepareToPlay(double sampleRate, int numChannels);
    void render(juce::AudioBuffer<float> &buffer);

    juce::AudioProcessorValueTreeState &getParameters() noexcept;

private:
    juce::AudioProcessorValueTreeState &parameters;
    double sampleRate = 0.0;
    Delay delay;
    ReverbFX reverb;
    int activeEffect;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXModel)
};
 