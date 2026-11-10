// Provides a reverb effect for the audio processor.
#pragma once

class ReverbFX {

public:
    ReverbFX(juce::AudioProcessorValueTreeState &state)
        : parameters(state)
    {
    }

    void prepareToPlay(double newSampleRate, int numChannels)
    {
        sampleRate = newSampleRate;
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

    void render(juce::AudioBuffer<float> &buffer)
    {
        if (sampleRate <= 0.0 || buffer.getNumChannels() == 0)
        {
            return;
        }

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
    }

private: 
    juce::AudioProcessorValueTreeState &parameters;
    double sampleRate = 0.0;
    juce::Reverb reverb;
};