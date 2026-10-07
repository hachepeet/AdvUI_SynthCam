#include "SynthView.h"

SynthView::SynthView()
{
    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(volumeSlider);

    for (auto *slider : {&frequencySlider, &volumeSlider})
    {
        slider->setSliderStyle(juce::Slider::LinearHorizontal);
        slider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    }

    frequencySlider.setRange(50.0, 5000.0);
    frequencySlider.setSkewFactorFromMidPoint(500.0);
    volumeSlider.setRange(0.0, 1.0);
}

void SynthView::paint(juce::Graphics &graphics)
{
    graphics.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SynthView::resized()
{
    auto area = getLocalBounds().reduced(20);
    frequencySlider.setBounds(area.removeFromTop(100));
    volumeSlider.setBounds(area.removeFromTop(100));
}

juce::Slider &SynthView::getFrequencySlider() noexcept
{
    return frequencySlider;
}

juce::Slider &SynthView::getVolumeSlider() noexcept
{
    return volumeSlider;
}
