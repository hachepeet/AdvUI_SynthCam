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

    // old code with just making 20 identical sine oscillators
    // auto numberOfOscillators = 20;                                                 

    // for (auto i = 0; i < numberOfOscillators; ++i){
    //     auto* oscillator = new Oscillator(sineTable);                                                  
    //     oscillator->setFrequency (frequency, (float) sampleRate);           
    //     oscillators.add (oscillator);
    // }

    // level = 0.25f / (float) numberOfOscillators;  

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

void SynthModel::render(juce::AudioBuffer<float> &buffer, float frequency, int waveform,
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

    // old wavetable code
    // for (auto oscillatorIndex = 0; oscillatorIndex < oscillators.size(); ++oscillatorIndex){
    //     auto* oscillator = oscillators.getUnchecked(oscillatorIndex);
    //     oscillator->setFrequency(frequency, (float) sampleRate);
    //     for (auto sample = 0; sample < buffer.getNumSamples(); ++sample){
    //         auto levelSample = oscillator->getNextSample() * level;
    //         for (auto channel = 0; channel < buffer.getNumChannels(); ++channel){
    //             buffer.addSample(channel, sample, levelSample);
    //         }
    //     }
    // }

    // old sine wave code
    // const auto frequencyInHz = static_cast<double>(*frequency);
    // const auto level = static_cast<float>(*waveform);
    // const auto angleDelta = frequencyInHz / sampleRate * juce::MathConstants<double>::twoPi;

    // for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    // {
    //     const auto currentSample = static_cast<float>(std::sin(currentAngle)) * level;
    //     currentAngle += angleDelta;

    //     for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    //         buffer.setSample(channel, sample, currentSample);
    // }
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
