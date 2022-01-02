#include "Synth.h"

WavetableSynth::WavetableSynth(juce::AudioProcessorValueTreeState& params) : parameters(params)
{
    wavetableController = new WavetableController(parameters);
    adsrController = new ADSRController(parameters);
    adsrController->controllerIndex = 0;
}

WavetableSynth::~WavetableSynth()
{
    delete wavetableController;
    delete adsrController;
}

WavetableController* WavetableSynth::getWavetableController(int index)
{
    return wavetableController;
}

ADSRController* WavetableSynth::getADSRController(int index)
{
    return adsrController;
}
