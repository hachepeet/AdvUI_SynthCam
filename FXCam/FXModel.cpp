#include "FXModel.h"

FXModel::FXModel(juce::AudioProcessorValueTreeState &state)
    : parameters(state), delay(parameters), reverb(parameters)
{
}

void FXModel::prepareToPlay(double newSampleRate, int numChannels)
{
    sampleRate = newSampleRate;

    // delay things
    delay.prepareToPlay(newSampleRate, numChannels);

    // reverb things
    reverb.prepareToPlay(newSampleRate, numChannels);

}

void FXModel::render(juce::AudioBuffer<float> &buffer)
{
    if (sampleRate <= 0.0 || buffer.getNumChannels() == 0)
    {
        buffer.clear();
        return;
    }

    // delay things
    delay.render(buffer);

    // reverb things
    reverb.render(buffer);

}

juce::AudioProcessorValueTreeState &FXModel::getParameters() noexcept
{
    return parameters;
}
 