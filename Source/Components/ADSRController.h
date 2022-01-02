#pragma once

#include <JuceHeader.h>
#include "BaseController.h"
#include "../Envelopes/AHDSR.h"

class ADSRController : public BaseController
{
public:
    ADSRController(juce::AudioProcessorValueTreeState&);
    ~ADSRController();

    int controllerIndex;
    float updateTableIndex(float tableIndex, bool noteOn);
    void prepareToPlay(double) override;

    float getModulatedParameter(juce::String &parameterID, float tableIndex);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AHDSREnvelope envelope;
    float tableDelta;

    juce::Slider volumeModSlider;

    juce::Slider attackSlider;
    juce::Slider holdSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::String getModulatedParameterId(juce::String &parameterID);
    void parameterChanged(const juce::String &parameterID, float newValue) override;
};
