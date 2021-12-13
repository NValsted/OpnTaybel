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

WavetableVoice::WavetableVoice(WavetableSynth* parent)
{
    synthPtr = parent;
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
    tailOff = 0.0f;

    float initialFrequency = (float)juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    float frequencyOffset = synthPtr->getWavetableController(0)->getFrequencyOffset();
    unisonVoiceCollection->updateTableDeltas(offsetFrequency(initialFrequency, frequencyOffset));
}

void WavetableVoice::stopNote(float velocity, bool allowTailOff)
{
    if (allowTailOff)
    {
        if (tailOff == 0.0f) { tailOff = 1.0f; }
    }
    else
    {
        clearCurrentNote();
    }
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
    phaseOffset = oscillator->wrapTableIndex(
        synthPtr->getWavetableController(0)->getPhaseOffset()
    );
    volume = velocity * synthPtr->getWavetableController(0)->getVolume();
    panLR = synthPtr->getWavetableController(0)->getPanLR();
    unisonStereo = unisonVoiceCollection->getStereoOffset();
    unisonVoices = unisonVoiceCollection->getVoices();

    if ((*unisonVoices)[0]->tableDelta != 0.0f)
    {
        float currentSamplePair[2] = { 0.0f, 0.0f };
        if (tailOff > 0.0f)
        {
            while (--numSamples >= 0)
            {
                currentSamplePair[0] = 0.0f; currentSamplePair[1] = 0.0f;
                for (int i = 0; i < unisonVoices->size(); i++)
                {
                    UnisonVoice* uVoice = (*unisonVoices)[i];
                    
                    currentSamplePair[0] += oscillator->getSample(
                        oscillator->wrapTableIndex(uVoice->tableIdx + phaseOffset + unisonStereo)
                    ) * uVoice->volume * volume * tailOff;
                    currentSamplePair[1] += oscillator->getSample(
                        oscillator->wrapTableIndex(uVoice->tableIdx + phaseOffset - unisonStereo)
                    ) * uVoice->volume * volume * tailOff;
                    
                    uVoice->tableIdx += uVoice->tableDelta;
                    uVoice->tableIdx = oscillator->wrapTableIndex(uVoice->tableIdx);
                }
                currentSamplePair[0] *= panLR[0]; currentSamplePair[1] *= panLR[1];
                for (int i = 0; i < 2; i++) {
                    outputBuffer.addSample(i, startSample, currentSamplePair[i]);
                }
                ++startSample;

                tailOff *= 0.99f;
                if (tailOff <= 0.005f)
                {
                    clearCurrentNote();
                    (*unisonVoices)[0]->tableDelta = 0.0f;
                    break;
                }
            }
        }
        else
        {
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
                currentSamplePair[0] *= panLR[0]; currentSamplePair[1] *= panLR[1];
                for (int i = 0; i < 2; i++) {
                    outputBuffer.addSample(i, startSample, currentSamplePair[i]);
                }
                ++startSample;
            }
        }
    }
}
