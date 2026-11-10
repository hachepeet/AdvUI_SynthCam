// Provides oscillator helpers for the synthesizer.
/*
References: 
JUCE Wavetable Synth Tutorial: https://juce.com/tutorials/tutorial_wavetable_synth/ 
*/
#pragma once 

#include <cmath>
#include <juce_core/system/juce_PlatformDefs.h>

class Oscillator {

public: 
    Oscillator (const juce::AudioSampleBuffer& wavetableToUse) : wavetable (wavetableToUse), tableSize (wavetable.getNumSamples() - 1) 
    {
        jassert (wavetable.getNumChannels() == 1);
    }

    void setFrequency(float frequency, float sampleRate){
        auto tableSizeOverSampleRate = (float) tableSize / sampleRate;
        tableDelta = frequency * tableSizeOverSampleRate;
    }

    forcedinline float getNextSample() noexcept{
        auto index0 = (unsigned int) currentIndex;              
        auto index1 = index0 + 1;
        auto frac = currentIndex - (float) index0;              
        auto* table = wavetable.getReadPointer (0);             
        auto value0 = table[index0];
        auto value1 = table[index1];
        auto currentSample = value0 + frac * (value1 - value0); 
        if ((currentIndex += tableDelta) > (float) tableSize) {
            currentIndex -= (float) tableSize;
        } 
        return currentSample;
    }

private:
    const juce::AudioSampleBuffer& wavetable;
    float currentIndex = 0.0f;
    float tableDelta = 0.0f;
    const int tableSize;

};
