#pragma once

#include <JuceHeader.h>
#include "UnisonVoice.h"
#include "Synth.h"

struct WavetableSound : public juce::SynthesiserSound
{
    WavetableSound();
    ~WavetableSound();
    
    bool appliesToNote(int) override;
    bool appliesToChannel(int) override;
};

class WavetableVoice : public juce::SynthesiserVoice
{
public:
    WavetableVoice(WavetableSynth* parent, int index);
    ~WavetableVoice();

    int voiceIndex;

    bool canPlaySound(juce::SynthesiserSound* sound) override;

    void startNote(int midiNoteNumber, float velocity,
                   juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;

    void updateTableDelta();
    void pitchWheelMoved(int) override;
    void controllerMoved(int, int) override;

    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

private:
    WavetableSound* wavetableSound;
    WavetableSynth* synthPtr;
    WavetableOscillator* oscillator;

    std::unique_ptr<UnisonVoiceCollection> unisonVoiceCollection;
    juce::OwnedArray<UnisonVoice>* unisonVoices;
    float tableIdx = 0.0f, tableDelta = 0.0f, volume = 0.0f;
    float ahdsrTableIndex = 0.0f;
    float phaseOffset = 0.0f, unisonStereo = 0.0f, velocity = 0.0f;
    juce::Array<float> panLR;
};
