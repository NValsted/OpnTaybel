#include "BaseController.h"

BaseController::BaseController(juce::AudioProcessorValueTreeState& params) : parameters(params)
{
}

BaseController::~BaseController()
{
    sliderAttachments.clear();
}

void BaseController::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate;
}

void BaseController::addSlider(juce::Slider& slider, juce::String paramID)
{
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    sliderAttachments.add(new SliderAttachment(parameters, paramID, slider));
}

void BaseController::drawTable(juce::Graphics& g, BaseTable* baseTable, juce::Rectangle<int> bounds)
{
    if (baseTable == nullptr)
        return;

    juce::Range<float> tableRange = baseTable->getTableRange();
    int left = bounds.getX();
    int right = bounds.getRight();
    
    float bottom = (float)bounds.getBottom();
    float top = (float)bounds.getY();

    int tableSize = baseTable->tableSize;
    for (int i = 0; i < tableSize - 1; i++)
    {
        g.drawLine(
            juce::jmap(i, 0, tableSize, left, right),
            juce::jmap(baseTable->getSample(i), tableRange.getStart(), tableRange.getEnd(), bottom, top),
            juce::jmap(i + 1, 0, tableSize, left, right),
            juce::jmap(baseTable->getSample(i + 1), tableRange.getStart(), tableRange.getEnd(), bottom, top)
        );
    }
}
