#include "SynthView.h"
#include "Theme.h"

SynthView::SynthView()
{
    addAndMakeVisible(cameraView);
    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(waveformSlider);

    for (auto *slider : {&frequencySlider, &waveformSlider})
    {
        slider->setSliderStyle(juce::Slider::LinearHorizontal);
        slider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    }

    frequencySlider.setRange(220.0, 880.0);
    frequencySlider.setSkewFactorFromMidPoint(std::sqrt(220.0 * 880.0));
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
    cameraView.setBounds(area.removeFromTop(220));
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

void SynthView::setHandPositionCallback(std::function<void(float, float, bool, bool)> callback)
{
    cameraView.setHandPositionCallback(std::move(callback));
}
