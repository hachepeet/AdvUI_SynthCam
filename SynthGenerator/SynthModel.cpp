#include "SynthModel.h"
#include <cmath>

SynthModel::SynthModel(juce::AudioProcessorValueTreeState &state)
    : parameters(state),
      frequency(parameters.getRawParameterValue("frequency")),
      waveform(parameters.getRawParameterValue("waveform"))
{
    createSineWavetable();
    createSquareWavetable();
    createTriWavetable();
    createRampWavetable();
    createSawWavetable();
}

void SynthModel::prepareToPlay(double newSampleRate, float frequency, int waveform)
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

void SynthModel::render(juce::AudioBuffer<float> &buffer, float frequency, int waveform)
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

    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample){
        auto currentSample = oscillator->getNextSample() * 0.25f;
        for (auto channel = 0; channel < buffer.getNumChannels(); ++channel){
            buffer.setSample(channel, sample, currentSample);
        }
    }

}

void SynthModel::createSineWavetable() {
    sineTable.setSize (1, (int) tableSize + 1); // only need 1 channel, # of samples = tableSize + 1 for wrapping
    auto* samples = sineTable.getWritePointer (0);                                   

    // angleDelta = angle between each sample, depends on # of samples
    auto angleDelta = juce::MathConstants<double>::twoPi / (double) (tableSize - 1); 
    auto currentAngle = 0.0;

    // for each index of the wavetable, get the value of sine, assign that value to the sample in the buffer, and increment the current angle by delta
    for (unsigned int i = 0; i < tableSize; ++i){
        auto sample = std::sin (currentAngle);                                       
        samples[i] = (float) sample;
        currentAngle += angleDelta;
    }

    // copying the first sample to the end to wrap around
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

    // one cycle of a square wave: first half of table is all -1, second half is all +1
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
