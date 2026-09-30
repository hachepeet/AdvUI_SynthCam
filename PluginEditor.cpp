#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(volumeSlider);

    frequencySlider.setSliderStyle (juce::Slider::LinearHorizontal);
    frequencySlider.setTextBoxStyle (
        juce::Slider::TextBoxRight,
        false,
        80,
        20
    );

    volumeSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle (
        juce::Slider::TextBoxRight,
        false,
        80,
        20
    );

    frequencySlider.setRange(50.0, 5000.0);
    frequencySlider.setSkewFactorFromMidPoint (500.0);
    volumeSlider.setRange (0.0, 1.0);

    fqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (
        p.getParameters(), "frequency", frequencySlider
    );
    volAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (
        p.getParameters(), "volume", volumeSlider
    );
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // g.setColour (juce::Colours::white);
    // g.setFont (15.0f);
    // g.drawFittedText ("svp", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds().reduced(20);

    frequencySlider.setBounds(
        area.removeFromTop(100)
    );

    volumeSlider.setBounds(
        area.removeFromTop(100)
    );
}
