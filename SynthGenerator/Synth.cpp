#include "Synth.h"
#include "SynthView.h"

Synth::Synth()
{
    view = std::make_unique<SynthView>();
    addAndMakeVisible(*view);
}

Synth::~Synth() = default;

juce::Slider &Synth::getFrequencySlider() noexcept
{
    return view->getFrequencySlider();
}

juce::Slider &Synth::getWaveformSlider() noexcept
{
    return view->getWaveformSlider();
}

void Synth::setHandPositionCallback(std::function<void(float, float, bool, bool)> callback)
{
    view->setHandPositionCallback(std::move(callback));
}

void Synth::resized()
{
    view->setBounds(getLocalBounds());
}
