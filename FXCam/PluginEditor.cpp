#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    view = std::make_unique<FXView>();
    addAndMakeVisible(*view);

    // fqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getModel().getParameters(), "frequency", view->getFrequencySlider());
    // volAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getModel().getParameters(), "volume", view->getVolumeSlider());
    delayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getModel().getParameters(), "delayTime", view->getDelaySlider());
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getModel().getParameters(), "dryWet", view->getDryWetSlider());

    setSize(800, 800);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() = default;

void AudioPluginAudioProcessorEditor::resized()
{
    view->setBounds(getLocalBounds());
}
 