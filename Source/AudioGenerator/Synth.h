#pragma once

#include <JuceHeader.h>
#include "../Components/OscillatorController.h"
#include "../Components/ADSRController.h"

class WavetableSynth : public juce::Synthesiser
{
public:
    WavetableSynth(juce::AudioProcessorValueTreeState&);
    ~WavetableSynth();

    WavetableController* getWavetableController(int index);
    ADSRController* getADSRController(int index);
    juce::AudioProcessorValueTreeState& parameters;

private:
    WavetableController* wavetableController;
    ADSRController* adsrController;
};
