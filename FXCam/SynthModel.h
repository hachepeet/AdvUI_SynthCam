#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class SynthModel final
{
public:
    explicit SynthModel(juce::AudioProcessorValueTreeState &parameters);

    void prepareToPlay(double sampleRate);
    void render(juce::AudioBuffer<float> &buffer);

    juce::AudioProcessorValueTreeState &getParameters() noexcept;

private:
    juce::AudioProcessorValueTreeState &parameters;
    std::atomic<float> *frequency = nullptr;
    std::atomic<float> *volume = nullptr;
    double sampleRate = 0.0;
    double currentAngle = 0.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthModel)
};
