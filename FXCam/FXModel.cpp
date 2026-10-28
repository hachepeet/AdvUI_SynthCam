#include "FXModel.h"

FXModel::FXModel(juce::AudioProcessorValueTreeState &state)
    : parameters(state), delay(parameters)
    //   ,volume(parameters.getRawParameterValue("volume"))
{
}

void FXModel::prepareToPlay(double newSampleRate, int numChannels)
{
    sampleRate = newSampleRate;

    // delay things
    delay.prepareToPlay(newSampleRate, numChannels);

    // reverb things
    reverb.setSampleRate(newSampleRate);
    reverb.reset();

    juce::Reverb::Parameters reverbParams;
    reverbParams.roomSize = 0.5f; // will later be changed based on user input
    reverbParams.damping = 0.5f;
    reverbParams.wetLevel = 0.5f; // will later be changed based on user input
    reverbParams.dryLevel = 0.5f; // will later be changed based on user input
    reverbParams.width = 1.0f;
    reverbParams.freezeMode = 0.0f;
    reverb.setParameters(reverbParams);
}

void FXModel::render(juce::AudioBuffer<float> &buffer)
{
    if (sampleRate <= 0.0 || buffer.getNumChannels() == 0)
    {
        buffer.clear();
        return;
    }

    // KEEP COMMENTED OUT
    // const auto volume = parameters.getRawParameterValue("volume")->load();

    // for (int channel = 0; channel < buffer.getNumChannels(); ++channel){
    //     buffer.applyGain(channel, 0, buffer.getNumSamples(), volume);
    // }

    // delay things
    delay.render(buffer);

    // reverb things
    const auto roomSize = parameters.getRawParameterValue("roomSize")->load();
    const auto reverbDryWetLevel = parameters.getRawParameterValue("reverbDryWet")->load();
    auto reverbParams = reverb.getParameters(); 

    reverbParams.roomSize = roomSize;
    reverbParams.wetLevel = reverbDryWetLevel;
    reverbParams.dryLevel = 1.0f - reverbDryWetLevel;
    reverb.setParameters(reverbParams);

    if (buffer.getNumChannels() == 1){
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
    }
    else if (buffer.getNumChannels() == 2){
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    }

    // KEEP COMMENTED OUT
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
 