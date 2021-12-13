#include "WheelSliders.h"

ModWheel::ModWheel()
{
    setSliderStyle(juce::Slider::LinearBarVertical);
    setRange(0.0, 127.0, 1.0);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
}

ModWheel::~ModWheel()
{
}

PitchWheel::PitchWheel()
{
    setSliderStyle(juce::Slider::LinearBarVertical);
    setRange(0.0, 127.0, 1.0);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
}

PitchWheel::~PitchWheel()
{
}
