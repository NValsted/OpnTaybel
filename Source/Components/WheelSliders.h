#pragma once

#include <JuceHeader.h>

class ModWheel : public juce::Slider
{
public:
    ModWheel();
    ~ModWheel() override;
};

class PitchWheel : public juce::Slider
{
public:
    PitchWheel();
    ~PitchWheel() override;
};
