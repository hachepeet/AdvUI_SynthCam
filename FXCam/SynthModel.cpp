#include "SynthModel.h"

SynthModel::SynthModel(juce::AudioProcessorValueTreeState &state)
    : parameters(state),
      frequency(parameters.getRawParameterValue("frequency")),
      volume(parameters.getRawParameterValue("volume"))
{
}

void SynthModel::prepareToPlay(double newSampleRate)
{
    sampleRate = newSampleRate;
    currentAngle = 0.0;
}

void SynthModel::render(juce::AudioBuffer<float> &buffer)
{
    if (sampleRate <= 0.0 || buffer.getNumChannels() == 0)
    {
        buffer.clear();
        return;
    }

    const auto frequencyInHz = static_cast<double>(*frequency);
    const auto level = static_cast<float>(*volume);
    const auto angleDelta = frequencyInHz / sampleRate * juce::MathConstants<double>::twoPi;

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        const auto currentSample = static_cast<float>(std::sin(currentAngle)) * level;
        currentAngle += angleDelta;

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            buffer.setSample(channel, sample, currentSample);
    }
}

juce::AudioProcessorValueTreeState &SynthModel::getParameters() noexcept
{
    return parameters;
}
