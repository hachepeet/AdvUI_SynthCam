#include "FXModel.h"

FXModel::FXModel(juce::AudioProcessorValueTreeState &state)
    : parameters(state)
    //   ,volume(parameters.getRawParameterValue("volume"))
{
}

void FXModel::prepareToPlay(double newSampleRate, int numChannels)
{
    sampleRate = newSampleRate;

    delaySize = 2097152;
    delayMask = delaySize - 1;
    writePointer = 0;

    delayBuffer.setSize(numChannels, delaySize);
    delayBuffer.clear();
    // currentAngle = 0.0;
}

void FXModel::render(juce::AudioBuffer<float> &buffer)
{
    if (sampleRate <= 0.0 || buffer.getNumChannels() == 0 || delayBuffer.getNumChannels() == 0)
    {
        buffer.clear();
        return;
    }

    // const auto volume = parameters.getRawParameterValue("volume")->load();

    // for (int channel = 0; channel < buffer.getNumChannels(); ++channel){
    //     buffer.applyGain(channel, 0, buffer.getNumSamples(), volume);
    // }

    const auto delayTime = parameters.getRawParameterValue("delayTime")->load();
    const auto dryWetLevel = parameters.getRawParameterValue("dryWet")->load();

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
        auto readPointer = writePointer + (long)(delayTime * 0.001 * sampleRate); 
        readPointer &= delayMask;
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel){
            auto* pointerToInputData = buffer.getWritePointer(channel);
            auto* pointerToDelayData = delayBuffer.getWritePointer(channel);

            const auto inputSample = pointerToInputData[sample];
            const auto delayedSample = pointerToDelayData[readPointer];
            pointerToDelayData[writePointer] = std::tanh(inputSample + feedback * delayedSample); 
            pointerToInputData[sample] = inputSample + dryWetLevel * delayedSample; 

        }
        writePointer = writePointer - 1;
        writePointer &= delayMask;
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
 