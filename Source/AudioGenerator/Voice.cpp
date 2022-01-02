#include "Voice.h"
#include "../Utils.h"

WavetableSound::WavetableSound()
{
}

WavetableSound::~WavetableSound()
{
}

bool WavetableSound::appliesToNote(int)
{
    return true;
}

bool WavetableSound::appliesToChannel(int)
{
    return true;
}

WavetableVoice::WavetableVoice(WavetableSynth* parent, int index)
{
    synthPtr = parent;
    voiceIndex = index;
    oscillator = synthPtr->getWavetableController(0)->getOscillator();
    unisonVoiceCollection = std::make_unique<UnisonVoiceCollection>(synthPtr);
}

WavetableVoice::~WavetableVoice()
{
}

bool WavetableVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<WavetableSound*>(sound) != nullptr;
}

void WavetableVoice::startNote(int midiNoteNumber, float velocity,
                               juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    
    tableIdx = synthPtr->getWavetableController(0)->getRandomPhaseOffset();

    this->velocity = velocity;
    ahdsrTableIndex = 0.0f;

    float initialFrequency = (float)juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    float frequencyOffset = synthPtr->getWavetableController(0)->getFrequencyOffset();
    unisonVoiceCollection->updateTableDeltas(offsetFrequency(initialFrequency, frequencyOffset));
}

void WavetableVoice::stopNote(float velocity, bool allowTailOff)
{
    if (!allowTailOff) { clearCurrentNote(); }
}

void WavetableVoice::pitchWheelMoved(int)
{
}

void WavetableVoice::controllerMoved(int, int)
{
}

void WavetableVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer,
                                     int startSample, int numSamples)
{
    if (wavetableSound == nullptr) { return; }
    if (ahdsrTableIndex == 2048.0f && isPlayingButReleased()) { clearCurrentNote(); }

    phaseOffset = oscillator->wrapTableIndex(
        synthPtr->getWavetableController(0)->getPhaseOffset()
    );

    volume = velocity * synthPtr->getADSRController(0)->getModulatedParameter(
        juce::String("volume"), ahdsrTableIndex
    );

    panLR = synthPtr->getWavetableController(0)->getPanLR();

    unisonStereo = unisonVoiceCollection->getStereoOffset();
    unisonVoices = unisonVoiceCollection->getVoices();

    if (isVoiceActive())
    {
        float currentSamplePair[2] = { 0.0f, 0.0f };
        while (--numSamples >= 0)
        {
            currentSamplePair[0] = 0.0f; currentSamplePair[1] = 0.0f;
            for (int i = 0; i < unisonVoices->size(); i++)
            {
                UnisonVoice* uVoice = (*unisonVoices)[i];
                
                currentSamplePair[0] += oscillator->getSample(
                    oscillator->wrapTableIndex(uVoice->tableIdx + phaseOffset + unisonStereo)
                ) * uVoice->volume * volume;
                currentSamplePair[1] += oscillator->getSample(
                    oscillator->wrapTableIndex(uVoice->tableIdx + phaseOffset - unisonStereo)
                ) * uVoice->volume * volume;
                
                uVoice->tableIdx += uVoice->tableDelta;
                uVoice->tableIdx = oscillator->wrapTableIndex(uVoice->tableIdx);
            }
            ahdsrTableIndex = synthPtr->getADSRController(0)->updateTableIndex(
                ahdsrTableIndex, this->isKeyDown()
            );
            currentSamplePair[0] *= panLR[0]; currentSamplePair[1] *= panLR[1];
            for (int i = 0; i < 2; i++) {
                outputBuffer.addSample(i, startSample, currentSamplePair[i]);
            }
            ++startSample;
        }
    }
}
