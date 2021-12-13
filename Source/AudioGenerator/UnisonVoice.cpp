#include "UnisonVoice.h"

UnisonVoice::UnisonVoice()
{
}

UnisonVoice::~UnisonVoice()
{
}

UnisonVoiceCollection::UnisonVoiceCollection(WavetableSynth* parentSynth)
{
    voices.add(new UnisonVoice());
    synthPtr = parentSynth;
    
    juce::StringArray paramNames = {
        "unisonVoices",
        "unisonDetune",
        "unisonDetuneSemitone",
        "unisonBlend",
        "unisonStereo"
    };
    for (juce::String param : paramNames) { synthPtr->parameters.addParameterListener(param, this); }
}

UnisonVoiceCollection::~UnisonVoiceCollection()
{
}

void UnisonVoiceCollection::updateTableDeltas(float frequency)
{
    playingFrequency = frequency;
    WavetableOscillator* oscillator = synthPtr->getWavetableController(0)->getOscillator();

    float voiceSemitonesOffset = 0.0f;
    float detuneQuotient = 2.0f / ((float)voices.size() - 1.0f) * detune * detuneSemitones;
    if (voices.size() > 1) { voiceSemitonesOffset = - detune * detuneSemitones; }

    for (int i = 0; i < voices.size(); i++)
    {
        voices[i]->tableDelta = oscillator->getTableDelta(offsetFrequency(frequency, voiceSemitonesOffset));
        voiceSemitonesOffset += detuneQuotient;
    }
}

void UnisonVoiceCollection::updateTableDeltas()
{
    if (playingFrequency != 0.0f) { updateTableDeltas(playingFrequency); }
}

void UnisonVoiceCollection::updateBlend()
{
    for (int i = 0; i < voices.size(); ++i) { voices[i]->volume = blend; }
    voices[voices.size() >> 1]->volume = 1.0f;
    if (!(voices.size() & 0b1)) { voices[(voices.size() >> 1) - 1]->volume = 1.0f; }
}

float UnisonVoiceCollection::getStereoOffset()
{
    return stereoOffset;
}

juce::OwnedArray<UnisonVoice>* UnisonVoiceCollection::getVoices()
{
    return &voices;
}

void UnisonVoiceCollection::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == "unisonVoices")
    {
        while (newValue < voices.size()) { voices.removeLast(); }
        while (newValue > voices.size()) { voices.add(new UnisonVoice()); }
        updateTableDeltas();
        updateBlend();
    }
    else if (parameterID == "unisonDetune")
    {
        detune = newValue;
        updateTableDeltas();
    }
    else if (parameterID == "unisonDetuneSemitone")
    {
        detuneSemitones = newValue;
        updateTableDeltas();
    }
    else if (parameterID == "unisonBlend")
    {
        blend = newValue;
        updateBlend();
    }
    else if (parameterID == "unisonStereo")
    {
        stereoOffset = newValue;
    }
}
