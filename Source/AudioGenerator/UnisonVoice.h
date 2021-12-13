#pragma once

#include <JuceHeader.h>
#include "Synth.h"
#include "Oscillator.h"
#include "../Utils.h"

struct UnisonVoice
{
    UnisonVoice();
    ~UnisonVoice();
    float tableIdx = 0.0f, tableDelta = 0.0f, volume = 0.2f;
};

class UnisonVoiceCollection : private juce::AudioProcessorValueTreeState::Listener
{
public:
    UnisonVoiceCollection(WavetableSynth*);
    ~UnisonVoiceCollection();
    void updateTableDeltas(float frequency);
    float getStereoOffset();
    juce::OwnedArray<UnisonVoice>* getVoices();

private:
    WavetableSynth* synthPtr;
    juce::OwnedArray<UnisonVoice> voices;
    float playingFrequency = 0.0f;
    float detune = 1.0f, detuneSemitones = 0.0f, blend = 0.0f, stereoOffset = 0.0f;

    void updateTableDeltas();
    void updateBlend();
    void parameterChanged(const juce::String &parameterID, float newValue) override;
};
