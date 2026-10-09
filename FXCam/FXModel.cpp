#include "FXModel.h"

FXModel::FXModel(juce::AudioProcessorValueTreeState &state)
    : parameters(state),
      volume(parameters.getRawParameterValue("volume"))
{
}

void FXModel::prepareToPlay(double newSampleRate)
{
    sampleRate = newSampleRate;
    // currentAngle = 0.0;
}

void FXModel::render(juce::AudioBuffer<float> &buffer)
{
    if (sampleRate <= 0.0 || buffer.getNumChannels() == 0)
    {
        buffer.clear();
        return;
    }

    const auto volume = parameters.getRawParameterValue("volume")->load();

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel){
        buffer.applyGain(channel, 0, buffer.getNumSamples(), volume);
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

juce::AudioProcessorValueTreeState &FXModel::getParameters() noexcept
{
    return parameters;
}
