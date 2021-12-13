#include "Synth.h"

WavetableSynth::WavetableSynth(juce::AudioProcessorValueTreeState& params) : parameters(params)
{
    wavetableController = new WavetableController(parameters);
}

WavetableSynth::~WavetableSynth()
{
    delete wavetableController;
}

WavetableController* WavetableSynth::getWavetableController(int index)
{
    return wavetableController;
}
