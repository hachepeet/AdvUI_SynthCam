#include "SynthView.h"
#include "Theme.h"

SynthView::SynthView()
{
    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(waveformSlider);

    for (auto *slider : {&frequencySlider, &waveformSlider})
    {
        slider->setSliderStyle(juce::Slider::LinearHorizontal);
        slider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    }

    frequencySlider.setRange(50.0, 5000.0);
    frequencySlider.setSkewFactorFromMidPoint(500.0);
    waveformSlider.setRange(0.0, 4.0, 1.0);
    waveformSlider.setNumDecimalPlacesToDisplay(0);
    waveformSlider.setSliderSnapsToMousePosition(false);
}

void SynthView::paint(juce::Graphics &graphics)
{
    graphics.fillAll(SynthCamTheme::backgroundColour);
}

void SynthView::resized()
{
    auto area = getLocalBounds().reduced(20);
    frequencySlider.setBounds(area.removeFromTop(100));
    waveformSlider.setBounds(area.removeFromTop(100));
}

juce::Slider &SynthView::getFrequencySlider() noexcept
{
    return frequencySlider;
}

juce::Slider &SynthView::getWaveformSlider() noexcept
{
    return waveformSlider;
}
