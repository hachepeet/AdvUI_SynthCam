#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Delay.h"

class FXModel final
{
public:
    explicit FXModel(juce::AudioProcessorValueTreeState &parameters);

    void prepareToPlay(double sampleRate, int numChannels);
    void render(juce::AudioBuffer<float> &buffer);

    juce::AudioProcessorValueTreeState &getParameters() noexcept;

private:
    juce::AudioProcessorValueTreeState &parameters;
    // std::atomic<float> *frequency = nullptr;
    // std::atomic<float> *volume = nullptr;
    double sampleRate = 0.0;
    // double currentAngle = 0.0;
    Delay delay;
    // juce::AudioBuffer<float> delayBuffer;
    // int delaySize = 2097152;
    // int delayMask = delaySize - 1;
    // int writePointer = 0;
    // float feedback = 0.5f;
    juce::Reverb reverb;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXModel)
};
 