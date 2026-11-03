#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    view = std::make_unique<Synth>();
    addAndMakeVisible(*view);

    fqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getParameters(), "frequency", view->getFrequencySlider());
    waveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.getParameters(), "waveform", view->getWaveformSlider());

    view->setHandPositionCallback([this](float x, float y, bool handIsClosed, bool handIsDetected)
    {
        processorRef.setHandClosed(handIsClosed || !handIsDetected);
        auto &parameters = processorRef.getParameters();

        auto setParameter = [&parameters](const juce::String &parameterId, float value)
        {
            if (auto *parameter = dynamic_cast<juce::RangedAudioParameter *>(
                    parameters.getParameter(parameterId)))
            {
                parameter->setValueNotifyingHost(parameter->convertTo0to1(value));
            }
        };

        constexpr auto minimumFrequency = 220.0f;
        constexpr auto maximumFrequency = 880.0f;
        const auto frequency = minimumFrequency
                             * std::pow(maximumFrequency / minimumFrequency, x);
        setParameter("frequency", frequency);
        setParameter("waveform", juce::jmap(y, 0.0f, 4.0f));
    });

    setSize(400, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() = default;

void AudioPluginAudioProcessorEditor::resized()
{
    view->setBounds(getLocalBounds());
}
