#include "FXView.h"
#include "Theme.h"

FXView::FXView()
{
    // addAndMakeVisible(frequencySlider);
    addAndMakeVisible(volumeSlider);

    // for (auto *slider : {&frequencySlider, &volumeSlider})
    // {
    //     slider->setSliderStyle(juce::Slider::LinearHorizontal);
    //     slider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    // }

    // frequencySlider.setRange(50.0, 5000.0);
    // frequencySlider.setSkewFactorFromMidPoint(500.0);
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    volumeSlider.setRange(0.0, 1.0);
}

void FXView::paint(juce::Graphics &graphics)
{
    graphics.fillAll(SynthCamTheme::backgroundColour);
}

void FXView::resized()
{
    auto area = getLocalBounds().reduced(20);
    frequencySlider.setBounds(area.removeFromTop(100));
    volumeSlider.setBounds(area.removeFromTop(100));
}

// juce::Slider &FXView::getFrequencySlider() noexcept
// {
//     return frequencySlider;
// }

juce::Slider &FXView::getVolumeSlider() noexcept
{
    return volumeSlider;
}
