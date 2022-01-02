#pragma once

#include <JuceHeader.h>
#include "BaseController.h"
#include "../AudioGenerator/Oscillator.h"

class WavetableController : public BaseController
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
    
    float phaseOffset = 0.0f;
    float INV_RAND_MAX = 1.0f / (float)RAND_MAX;

    void drawWavetable(juce::Graphics&);
    void parameterChanged(const juce::String &parameterID, float newValue) override;
};
