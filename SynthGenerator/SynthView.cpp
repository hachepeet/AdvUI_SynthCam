#include "SynthView.h"
#include "Theme.h"

SynthView::SynthView()
{
    addAndMakeVisible(pluginTitle);
    addAndMakeVisible(camera);
    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(waveformSlider);

    addAndMakeVisible(sinLabel);
    addAndMakeVisible(sqLabel);
    addAndMakeVisible(triLabel);
    addAndMakeVisible(rampLabel);
    addAndMakeVisible(sawLabel);
    addAndMakeVisible(waveAxisLabel);

    addAndMakeVisible(fqMinLabel);
    addAndMakeVisible(fqMaxLabel);
    addAndMakeVisible(fqDefaultLabel);
    addAndMakeVisible(fqAxisLabel);

    pluginTitle.setText("SynthGenerator", juce::dontSendNotification);
    pluginTitle.setFont(juce::Font(24.0f));
    pluginTitle.setJustificationType(juce::Justification::centred);

    frequencySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    frequencySlider.setRange(220.0, 880.0);
    frequencySlider.setSkewFactorFromMidPoint(std::sqrt(220.0 * 880.0));
    waveformSlider.setSliderStyle(juce::Slider::LinearVertical);
    waveformSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 20);
    waveformSlider.setRange(0.0, 4.0, 1.0);
    waveformSlider.setNumDecimalPlacesToDisplay(0);
    waveformSlider.setSliderSnapsToMousePosition(false);

    sinLabel.setText("Sine", juce::dontSendNotification);
    sqLabel.setText("Square", juce::dontSendNotification);
    triLabel.setText("Triangle", juce::dontSendNotification);
    rampLabel.setText("Ramp", juce::dontSendNotification);
    sawLabel.setText("Saw", juce::dontSendNotification);
    waveAxisLabel.setText("Waveform", juce::dontSendNotification);

    fqMinLabel.setText("A3", juce::dontSendNotification);
    fqMaxLabel.setText("A5", juce::dontSendNotification);
    fqDefaultLabel.setText("A4", juce::dontSendNotification);
    fqAxisLabel.setText("Frequency / Pitch", juce::dontSendNotification);

    fqMinLabel.setJustificationType(juce::Justification::centredLeft);
    fqMaxLabel.setJustificationType(juce::Justification::centredRight);
    fqDefaultLabel.setJustificationType(juce::Justification::centred);

    sinLabel.setJustificationType(juce::Justification::centredLeft);
    sqLabel.setJustificationType(juce::Justification::centredLeft);
    triLabel.setJustificationType(juce::Justification::centredLeft);
    rampLabel.setJustificationType(juce::Justification::centredLeft);
    sawLabel.setJustificationType(juce::Justification::centredLeft);
}

void SynthView::paint(juce::Graphics &graphics)
{
    graphics.fillAll(SynthCamTheme::backgroundColour);
}

void SynthView::resized()
{
    auto area = getLocalBounds().reduced(20);
    pluginTitle.setBounds(area.removeFromTop(30));
    camera.setBounds(area.removeFromTop(220));
    int verticalSliderWidth = 40;
    int verticalSliderHeight = 240;
    int horizontalSliderWidth = 300;
    int horizontalSliderHeight = 40;
    auto sliderBounds = area.withSizeKeepingCentre(verticalSliderWidth + horizontalSliderWidth, verticalSliderHeight + horizontalSliderHeight);

    waveformSlider.setBounds(sliderBounds.getX(), sliderBounds.getY(), verticalSliderWidth, verticalSliderHeight);
    int interval = waveformSlider.getHeight() / 4;
    sinLabel.setBounds(waveformSlider.getX() - 55, waveformSlider.getY(), 70, 20);
    sqLabel.setBounds(waveformSlider.getX() - 55, waveformSlider.getY() + interval, 70, 20);
    triLabel.setBounds(waveformSlider.getX() - 55, waveformSlider.getY() + (2 * interval), 70, 20);
    rampLabel.setBounds(waveformSlider.getX() - 55, waveformSlider.getY() + (3 * interval), 70, 20);
    sawLabel.setBounds(waveformSlider.getX() - 55, waveformSlider.getBottom() - 20 / 2, 70, 20);
    waveAxisLabel.setBounds(waveformSlider.getX() - 150, waveformSlider.getY() + waveformSlider.getHeight() / 2 - 10, 100, 20);

    frequencySlider.setBounds(sliderBounds.getX() + verticalSliderWidth, sliderBounds.getBottom() - horizontalSliderHeight, horizontalSliderWidth, horizontalSliderHeight);
    int textBoxWidth = 80;
    int adaptedWidth = frequencySlider.getWidth() - textBoxWidth;
    fqMinLabel.setBounds(frequencySlider.getX(), frequencySlider.getBottom(), 50, 20);
    fqMaxLabel.setBounds(frequencySlider.getRight() - 130, frequencySlider.getBottom(), 50, 20);
    int defaultX = frequencySlider.getX() + adaptedWidth / 3 - 20;
    fqDefaultLabel.setBounds(defaultX, frequencySlider.getBottom(), 50, 20);
    fqAxisLabel.setBounds(frequencySlider.getX(), frequencySlider.getBottom() + 30, adaptedWidth, 20);
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
    camera.setHandPositionCallback(std::move(callback));
}
