#pragma once

#include <JuceHeader.h>
#include "../AudioGenerator/Oscillator.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class WavetableController : private juce::AudioProcessorValueTreeState::Listener,
                            public juce::Component
{
public:
    WavetableController(juce::AudioProcessorValueTreeState&);
    ~WavetableController();
    WavetableOscillator* getOscillator();

    float getFrequencyOffset();
    float getPhaseOffset();
    float getRandomPhaseOffset();
    float getVolume();
    juce::Array<float> getPanLR();
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    WavetableOscillator* wavetableOscillator;

    juce::Slider tableStateSlider;
    std::unique_ptr<SliderAttachment> tableStateAttachment;
    juce::TextButton openButton;

    juce::Slider pitchFineSlider;
    juce::Slider pitchCoarseSlider;
    juce::Slider pitchSemitoneSlider;
    juce::Slider pitchOctaveSlider;
    juce::Slider tablePhaseOffsetSlider;
    juce::Slider tablePhaseRandSlider;
    juce::Slider unisonVoicesSlider;
    juce::Slider unisonDetuneSlider;
    juce::Slider unisonDetuneSemitoneSlider;
    juce::Slider unisonBlendSlider;
    juce::Slider unisonStereoSlider;

    juce::Slider volumeSlider;
    juce::Slider panSlider;
    juce::OwnedArray<SliderAttachment> sliderAttachments;
    
    float phaseOffset = 0.0f;

    void addSlider(juce::Slider&, juce::String);
    void drawWavetable(juce::Graphics&);
    void parameterChanged(const juce::String &parameterID, float newValue) override;
};
