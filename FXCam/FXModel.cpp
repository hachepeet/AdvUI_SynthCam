// Implements the effects model and parameter state.
#include "FXModel.h"

FXModel::FXModel(juce::AudioProcessorValueTreeState &state)
    : parameters(state), delay(parameters), reverb(parameters)
{
}

void FXModel::prepareToPlay(double newSampleRate, int numChannels)
{
    sampleRate = newSampleRate;

    delay.prepareToPlay(newSampleRate, numChannels);
    reverb.prepareToPlay(newSampleRate, numChannels);

}

void FXModel::render(juce::AudioBuffer<float> &buffer)
{
    if (sampleRate <= 0.0 || buffer.getNumChannels() == 0)
    {
        buffer.clear();
        return;
    }

    activeEffect = static_cast<int>(parameters.getRawParameterValue("activeEffect")->load());

    if (activeEffect == 0) delay.render(buffer);
    else if (activeEffect == 1) reverb.render(buffer);

}

juce::AudioProcessorValueTreeState &FXModel::getParameters() noexcept
{
    return parameters;
}
 