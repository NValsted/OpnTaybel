#include "OscillatorController.h"

WavetableController::WavetableController(juce::AudioProcessorValueTreeState& params) : BaseController(params)
{
    wavetableOscillator = new WavetableOscillator(parameters);

    addAndMakeVisible(openButton);
    parameters.addParameterListener("wavetablePath", this);
    openButton.setButtonText("Load wavetable");
    openButton.onClick = [this] { wavetableOscillator->selectWaveTable(); };
    
    addAndMakeVisible(tableStateSlider);
    parameters.addParameterListener("tableState", this);
    tableStateSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    tableStateAttachment.reset(new SliderAttachment(parameters, "tableState", tableStateSlider));

    addSlider(pitchFineSlider, "pitchFine");
    addSlider(pitchCoarseSlider, "pitchCoarse");
    addSlider(pitchSemitoneSlider, "pitchSemitone");
    addSlider(pitchOctaveSlider, "pitchOctave");
    
    parameters.addParameterListener("tablePhase", this);
    addSlider(tablePhaseOffsetSlider, "tablePhase");
    addSlider(tablePhaseRandSlider, "tablePhaseRand");

    addSlider(unisonVoicesSlider, "unisonVoices");
    addSlider(unisonDetuneSlider, "unisonDetune");
    addSlider(unisonDetuneSemitoneSlider, "unisonDetuneSemitone");
    addSlider(unisonBlendSlider, "unisonBlend");
    addSlider(unisonStereoSlider, "unisonStereo");
    
    addSlider(volumeSlider, "volume");
    addSlider(panSlider, "pan");
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    panSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
}

WavetableController::~WavetableController()
{
    delete wavetableOscillator;
    sliderAttachments.clear();
}

WavetableOscillator* WavetableController::getOscillator()
{
    return wavetableOscillator;
}

float WavetableController::getFrequencyOffset()
{
    float frequencyOffset = 0.0f;
    for (auto paramID : { "pitchFine", "pitchCoarse", "pitchSemitone" })
    {
        float paramVal = parameters.getParameter(paramID)->getNormalisableRange()
        .convertFrom0to1(parameters.getParameter(paramID)->getValue());
        frequencyOffset += paramVal;
    }
    frequencyOffset += (float)(parameters.getParameter("pitchOctave")->getNormalisableRange()
    .convertFrom0to1(parameters.getParameter("pitchOctave")->getValue())) * 12.0f;
    return frequencyOffset;
}

float WavetableController::getPhaseOffset()
{
    return parameters.getParameter("tablePhase")->getNormalisableRange().convertFrom0to1(
        parameters.getParameter("tablePhase")->getValue()
    );
}

float WavetableController::getRandomPhaseOffset()
{
    return (float)rand() * INV_RAND_MAX
        * parameters.getParameter("tablePhaseRand")->getNormalisableRange().convertFrom0to1(
            parameters.getParameter("tablePhaseRand")->getValue()
        ) * wavetableOscillator->tableSize;
}

float WavetableController::getVolume()
{
    return parameters.getParameter("volume")->getValue();
}

juce::Array<float> WavetableController::getPanLR()
{
    float pan = parameters.getParameter("pan")->getValue();
    return {1.0f - pan, pan};
}

void WavetableController::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == "tableState")
    {
        wavetableOscillator->updateTableOffset((int)newValue);
        repaint();
    }
    else if (parameterID == "wavetablePath")
    {
        openButton.setButtonText(parameters.getParameter("wavetablePath")->getCurrentValueAsText());
        repaint();
    }
}

void WavetableController::paint(juce::Graphics& g)
{
    //drawTable(g, wavetableOscillator, getLocalBounds());
    drawWavetable(g);
}

void WavetableController::resized()
{
    float heightOver3 = getLocalBounds().getHeight() / 3;
    
    openButton.setBounds(0, 0, 80, heightOver3 - 10);
    tableStateSlider.setBounds(90, 0, 200, heightOver3);

    pitchFineSlider.setBounds(300, 0, 70, 10);
    pitchCoarseSlider.setBounds(300, 10, 70, 10);
    pitchSemitoneSlider.setBounds(300, 20, 70, 10);
    pitchOctaveSlider.setBounds(300, 30, 70, 10);
    tablePhaseOffsetSlider.setBounds(300, 40, 70, 10);
    tablePhaseRandSlider.setBounds(300, 50, 70, 10);
    unisonVoicesSlider.setBounds(300, 60, 70, 10);
    unisonDetuneSlider.setBounds(300, 70, 70, 10);
    unisonDetuneSemitoneSlider.setBounds(300, 80, 70, 10);
    unisonBlendSlider.setBounds(300, 90, 70, 10);
    unisonStereoSlider.setBounds(300, 100, 70, 10);

    volumeSlider.setBounds(370, 0, 30, 50);
    panSlider.setBounds(370, 50, 30, 50);
}

void WavetableController::drawWavetable(juce::Graphics& g)
{
    float heightOver3 = getLocalBounds().getHeight() / 3;
    int tableSize = (int)wavetableOscillator->tableSize;
    for (int i = 0; i < tableSize - 1; i++)
    {
        g.drawLine(  // TODO: wavetable is currently upside down - fix this
            juce::jmap(i, 0, tableSize, 0, getLocalBounds().getWidth() - 110),
            juce::jmap(
                wavetableOscillator->getActiveTableSlice()[i],
                wavetableOscillator->amplitudeRange.getStart(),
                wavetableOscillator->amplitudeRange.getEnd(),
                heightOver3,
                (float)getLocalBounds().getHeight()
            ),
            juce::jmap(i + 1, 0, tableSize, 0, getLocalBounds().getWidth() - 110),
            juce::jmap(
                wavetableOscillator->getActiveTableSlice()[i + 1],
                wavetableOscillator->amplitudeRange.getStart(),
                wavetableOscillator->amplitudeRange.getEnd(),
                heightOver3,
                (float)getLocalBounds().getHeight()
            )
        );
    }
}
