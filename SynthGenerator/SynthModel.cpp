#include "SynthModel.h"

SynthModel::SynthModel(juce::AudioProcessorValueTreeState &state)
    : parameters(state),
      frequency(parameters.getRawParameterValue("frequency")),
      volume(parameters.getRawParameterValue("volume"))
{
    createWavetable();
}

void SynthModel::prepareToPlay(double newSampleRate, float frequency, float volume)
{
    this->sampleRate = newSampleRate;
    auto numberOfOscillators = 20;                                                 

    for (auto i = 0; i < numberOfOscillators; ++i){
        auto* oscillator = new Oscillator(sineTable);                                                  
        oscillator->setFrequency (frequency, (float) sampleRate);           
        oscillators.add (oscillator);
    }

    level = 0.25f / (float) numberOfOscillators;  

}

void SynthModel::render(juce::AudioBuffer<float> &buffer, float frequency, float volume)
{
    if (sampleRate <= 0.0 || buffer.getNumChannels() == 0)
    {
        buffer.clear();
        return;
    }

    for (auto oscillatorIndex = 0; oscillatorIndex < oscillators.size(); ++oscillatorIndex){
        auto* oscillator = oscillators.getUnchecked(oscillatorIndex);
        oscillator->setFrequency(frequency, (float) sampleRate);
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample){
            auto levelSample = oscillator->getNextSample() * level;
            for (auto channel = 0; channel < buffer.getNumChannels(); ++channel){
                buffer.addSample(channel, sample, levelSample);
            }
        }
    }

    // const auto frequencyInHz = static_cast<double>(*frequency);
    // const auto level = static_cast<float>(*volume);
    // const auto angleDelta = frequencyInHz / sampleRate * juce::MathConstants<double>::twoPi;

    // for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    // {
    //     const auto currentSample = static_cast<float>(std::sin(currentAngle)) * level;
    //     currentAngle += angleDelta;

    //     for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    //         buffer.setSample(channel, sample, currentSample);
    // }
}

void SynthModel::createWavetable() {
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

juce::AudioProcessorValueTreeState &SynthModel::getParameters() noexcept
{
    return parameters;
}
