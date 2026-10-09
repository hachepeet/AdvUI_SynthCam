#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Oscillator.h"

class SynthModel final
{
public:
    explicit SynthModel(juce::AudioProcessorValueTreeState &parameters);

    void prepareToPlay(double newSampleRate, float frequency, float volume);
    void render(juce::AudioBuffer<float> &buffer, float frequency, float volume);
    void createWavetable();

    juce::AudioProcessorValueTreeState &getParameters() noexcept;

private:
    juce::AudioProcessorValueTreeState &parameters;
    std::atomic<float> *frequency = nullptr;
    std::atomic<float> *volume = nullptr;
    double sampleRate = 0.0;
    // double currentAngle = 0.0;
    float level = 0.0f;
    juce::OwnedArray<Oscillator> oscillators;
    const unsigned int tableSize = 1 << 7;
    juce::AudioSampleBuffer sineTable;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthModel)
};
