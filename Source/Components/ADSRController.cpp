#include "ADSRController.h"

ADSRController::ADSRController(juce::AudioProcessorValueTreeState& params) : BaseController(params),
                                                                             envelope(params)
{
    addSlider(attackSlider, "attack");
    addSlider(holdSlider, "hold");
    addSlider(decaySlider, "decay");
    addSlider(sustainSlider, "sustain");
    addSlider(releaseSlider, "release");

    addSlider(volumeModSlider, "AHDSR0-volume");

    for (auto paramID: {"attack", "hold", "decay", "sustain", "release"})
    {
        parameters.addParameterListener(paramID, this);
    }
}

ADSRController::~ADSRController()
{
}

void ADSRController::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate;
    envelope.prepareToPlay(sampleRate);
    tableDelta = envelope.getTableDelta();
}

void ADSRController::paint(juce::Graphics& g)
{
    drawTable(g, &envelope, getLocalBounds());
}

void ADSRController::resized()
{
    attackSlider.setBounds(0, 0, 70, 10);
    holdSlider.setBounds(0, 10, 70, 10);
    decaySlider.setBounds(0, 20, 70, 10);
    sustainSlider.setBounds(0, 30, 70, 10);
    releaseSlider.setBounds(0, 40, 70, 10);

    volumeModSlider.setBounds(80, 0, 70, 10);
}

void ADSRController::parameterChanged(const juce::String &parameterID, float newValue)
{
    for (auto paramID: {"attack", "hold", "decay", "sustain", "release"})
    {
        if (parameterID == paramID)
        {
            envelope.updateEnvelope();
            tableDelta = envelope.getTableDelta();
            repaint();
            break;
        }
    }
}

float ADSRController::updateTableIndex(float tableIndex, bool noteOn)
{
    return envelope.wrapTableIndex(tableIndex + tableDelta, noteOn);
}

juce::String ADSRController::getModulatedParameterId(juce::String &parameterID)
{
    return juce::String("AHDSR" + juce::String(controllerIndex) + "-" + parameterID);
}

float ADSRController::getModulatedParameter(juce::String &parameterID,
                                            float tableIndex)
{
    float modFactor = envelope.getSample(tableIndex)
        * dynamic_cast<juce::AudioParameterFloat*>(
            parameters.getParameter(getModulatedParameterId(parameterID))
        )->get();

    juce::NormalisableRange<float> paramRange = parameters.getParameter(parameterID)->getNormalisableRange();
    float modVal = (paramRange.end - paramRange.start) * modFactor;

    float value = (float)(parameters.getParameter(parameterID)->getNormalisableRange()
    .convertFrom0to1(parameters.getParameter(parameterID)->getValue()));

    return juce::jmax<float>(paramRange.start, juce::jmin<float>(paramRange.end, value + modVal));
}
