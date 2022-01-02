#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/WheelSliders.h"

class OpnTaybelAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    OpnTaybelAudioProcessorEditor(OpnTaybelAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~OpnTaybelAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    ModWheel modWheel;
    PitchWheel pitchWheel;
    WavetableController* wavetableController;
    ADSRController* adsrController;

private:
    OpnTaybelAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpnTaybelAudioProcessorEditor)
    juce::AudioProcessorValueTreeState& parameters;
};
