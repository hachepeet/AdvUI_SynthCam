// Implements the synthesizer model and parameters.
/* 
References: 
JUCE Wavetable Synth Tutorial: https://juce.com/tutorials/tutorial_wavetable_synth/
Synth Notes Square and Triangle Oscillators: https://synthnotes.ucsd.edu/wp7/index.php/2019/10/01/simple-square-and-triangle-oscillators/
Synth Notes Ramp and Sawtooth Oscillators: https://synthnotes.ucsd.edu/wp7/index.php/2019/10/01/simple-ramp-and-sawtooth-oscillators/ 
*/
#include "SynthModel.h"
#include <cmath>

SynthModel::SynthModel(juce::AudioProcessorValueTreeState &state)
    : parameters(state)
{
    createSineWavetable();
    createSquareWavetable();
    createTriWavetable();
    createRampWavetable();
    createSawWavetable();
}

void SynthModel::prepareToPlay(double newSampleRate, float frequency)
{
    this->sampleRate = newSampleRate;

    oscillators.clear();
    oscillators.add(new Oscillator(sineTable));
    oscillators.add(new Oscillator(triTable));
    oscillators.add(new Oscillator(squareTable));
    oscillators.add(new Oscillator(rampTable));
    oscillators.add(new Oscillator(sawTable));

    for (auto* oscillator : oscillators){
        oscillator->setFrequency(frequency, (float) sampleRate);
    }

}

void SynthModel::render(juce::AudioBuffer<float> &buffer, float frequency, float waveform,
                        float gainStart, float gainEnd, int activeSamples)
{
    if (sampleRate <= 0.0 || buffer.getNumChannels() == 0)
    {
        buffer.clear();
        return;
    }

    auto waveformIndex = juce::jlimit(0, oscillators.size() - 1, juce::roundToInt(waveform));
    auto* oscillator = oscillators.getUnchecked(waveformIndex);
    oscillator->setFrequency(frequency, (float) sampleRate);
    buffer.clear();

    const auto samplesToRender = activeSamples < 0
                                ? buffer.getNumSamples()
                                : juce::jmin(activeSamples, buffer.getNumSamples());

    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample){
        const auto gain = sample < samplesToRender
                            ? juce::jmap(static_cast<float>(sample),
                                         0.0f, static_cast<float>(juce::jmax(1, samplesToRender - 1)),
                                         gainStart, gainEnd)
                            : 0.0f;
        auto currentSample = oscillator->getNextSample() * 0.25f * gain;
        for (auto channel = 0; channel < buffer.getNumChannels(); ++channel){
            buffer.setSample(channel, sample, currentSample);
        }
    }

}

void SynthModel::createSineWavetable() {
    sineTable.setSize (1, (int) tableSize + 1);
    auto* samples = sineTable.getWritePointer (0);                                   

    auto angleDelta = juce::MathConstants<double>::twoPi / (double) (tableSize - 1); 
    auto currentAngle = 0.0;

    for (unsigned int i = 0; i < tableSize; ++i){
        auto sample = std::sin (currentAngle);                                       
        samples[i] = (float) sample;
        currentAngle += angleDelta;
    }

    samples[tableSize] = samples[0];
}

void SynthModel::createTriWavetable() {
    triTable.setSize(1, (int) tableSize + 1);
    auto* samples = triTable.getWritePointer(0);

    for (unsigned int i = 0; i < tableSize; ++i){
        auto t = (float) i / (float) tableSize;
        samples[i] = 4.0f * std::abs(2.0f * (t - std::floor(t + 0.5f))) - 1.0f;
    }

    samples[tableSize] = samples[0];
}

void SynthModel::createSquareWavetable() {
    squareTable.setSize(1, (int) tableSize + 1);
    auto* samples = squareTable.getWritePointer(0);

    for (unsigned int i = 0; i < tableSize; ++i){
        if (i < tableSize / 2) samples[i] = -1.0f;
        else samples [i] = 1.0f;
    }

    samples[tableSize] = samples[0];
}

void SynthModel::createRampWavetable() {
    rampTable.setSize(1, (int) tableSize + 1);
    auto* samples = rampTable.getWritePointer(0);

    for (unsigned int i = 0; i < tableSize; ++i){
        auto t = (float) i / (float) tableSize;
        samples[i] = 2.0f * t - 1.0f;
    }

    samples[tableSize] = samples[0];
}

void SynthModel::createSawWavetable() {
    sawTable.setSize(1, (int) tableSize + 1);
    auto* samples = sawTable.getWritePointer(0);

    for (unsigned int i = 0; i < tableSize; ++i){
        auto t = (float) i / (float) tableSize;
        samples[i] = -2.0f * t + 1.0f;
    }

    samples[tableSize] = samples[0];
}

juce::AudioProcessorValueTreeState &SynthModel::getParameters() noexcept
{
    return parameters;
}
