#pragma once

#include <JuceHeader.h>
#include "../BaseTable.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class BaseController : protected juce::AudioProcessorValueTreeState::Listener,
                       public juce::Component
{
public:
    BaseController(juce::AudioProcessorValueTreeState&);
    ~BaseController();
    virtual void prepareToPlay(double);

protected:
    float sampleRate = 0.0f;
    juce::AudioProcessorValueTreeState& parameters;
    juce::OwnedArray<SliderAttachment> sliderAttachments;
    
    void addSlider(juce::Slider&, juce::String);
    void drawTable(juce::Graphics&, BaseTable*, juce::Rectangle<int>);
};
