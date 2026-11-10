// Declares the synthesizer model and parameters.
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Oscillator.h"

class SynthModel final
{
public:
    explicit SynthModel(juce::AudioProcessorValueTreeState &parameters);

    void prepareToPlay(double newSampleRate, float frequency, int waveform);
    void render(juce::AudioBuffer<float> &buffer, float frequency, int waveform,
                float gainStart = 1.0f, float gainEnd = 1.0f, int activeSamples = -1);
    void createSineWavetable();
    void createTriWavetable();
    void createSquareWavetable();
    void createRampWavetable();
    void createSawWavetable();

    juce::AudioProcessorValueTreeState &getParameters() noexcept;

private:
    juce::AudioProcessorValueTreeState &parameters;
    std::atomic<float> *frequency = nullptr;
    std::atomic<float> *waveform = nullptr;
    double sampleRate = 0.0;
    // double currentAngle = 0.0;
    float level = 0.0f;
    juce::OwnedArray<Oscillator> oscillators;
    const unsigned int tableSize = 1 << 7;
    juce::AudioSampleBuffer sineTable;
    juce::AudioSampleBuffer triTable;
    juce::AudioSampleBuffer squareTable;
    juce::AudioSampleBuffer rampTable;
    juce::AudioSampleBuffer sawTable;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthModel)
};
 