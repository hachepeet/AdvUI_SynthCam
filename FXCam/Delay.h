// Provides a delay effect for the audio processor.
#pragma once 

#include <juce_audio_processors/juce_audio_processors.h>

class Delay {

public: 
    Delay(juce::AudioProcessorValueTreeState &state)
        : parameters(state)
    {
    }

    void prepareToPlay(double newSampleRate, int numChannels)
    {
        sampleRate = newSampleRate;
        delaySize = 2097152;
        delayMask = delaySize - 1;
        writePointer = 0;

        delayBuffer.setSize(numChannels, delaySize);
        delayBuffer.clear();
    }

    void render(juce::AudioBuffer<float> &buffer)
    {
        if (sampleRate <= 0.0 || buffer.getNumChannels() == 0 || delayBuffer.getNumChannels() < buffer.getNumChannels())
        {
            return;
        }
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
    }

private:
    juce::AudioProcessorValueTreeState &parameters;
    double sampleRate = 0.0;
    juce::AudioBuffer<float> delayBuffer;
    int delaySize = 2097152;
    int delayMask = delaySize - 1;
    int writePointer = 0;
    float feedback = 0.5f;
};
