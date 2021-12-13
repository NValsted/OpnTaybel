#pragma once

#include <JuceHeader.h>
#include "../Components/OscillatorController.h"

class WavetableSynth : public juce::Synthesiser
{
public:
    WavetableSynth(juce::AudioProcessorValueTreeState&);
    ~WavetableSynth();

    WavetableController* getWavetableController(int index);
    juce::AudioProcessorValueTreeState& parameters;

private:
    WavetableController* wavetableController;
};
