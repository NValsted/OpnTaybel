#include "Parameters.h"

juce::Array<juce::File> getWavetableFiles()
{
    return juce::File::getSpecialLocation(  // Might be a good idea to sort the files by name
        juce::File::userDocumentsDirectory
    ).getChildFile("OpnTaybel/Tables").findChildFiles(
        juce::File::findFiles, false, "*.wav"
    );
}

std::unordered_map<juce::String, int> getWavetableMap()
{
    juce::Array<juce::File> wavetableFiles = getWavetableFiles();
    
    std::unordered_map<juce::String, int> wavetableMap;
    for (int i = 0; i < wavetableFiles.size(); i++)
    {
        wavetableMap.insert({ wavetableFiles[i].getFileName(), i });
    }

    return wavetableMap;
}

juce::Array<juce::RangedAudioParameter*> getModulableParameters()
{
    return juce::Array<juce::RangedAudioParameter*>({
        new juce::AudioParameterInt("tableState", "Wavetable state", 0, 255, 0),
        new juce::AudioParameterFloat("tablePhase", "Wavetable phase offset", juce::NormalisableRange<float>(0.0f, (float)_tableSize), 0.0f),
        new juce::AudioParameterFloat("tablePhaseRand", "Wavetable phase randomization", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f),
        
        new juce::AudioParameterInt("unisonVoices", "Unison voices", 1, 16, 1),
        new juce::AudioParameterFloat("unisonDetune", "Unison detune", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.5f), 0.25f),
        new juce::AudioParameterInt("unisonDetuneSemitone", "Max unison detune in semitones", 1, 24, 1),
        new juce::AudioParameterFloat("unisonBlend", "Unison blend", 0.0f, 1.0f, 0.0f),
        new juce::AudioParameterFloat("unisonStereo", "Unison stereo width", juce::NormalisableRange<float>(0.0f, (float)_tableSize / 4.0f, 1.0f, 0.5f), 0.0f),
        
        new juce::AudioParameterFloat("pitchFine", "Fine pitch offset", -1.0f, 1.0f, 0.0f),
        new juce::AudioParameterFloat("pitchCoarse", "Coarse pitch offset", -64.0f, 64.0f, 0.0f),
        new juce::AudioParameterInt("pitchSemitone", "Pitch semitone offset", -12, 12, 0),
        new juce::AudioParameterInt("pitchOctave", "Pitch octave offset", -4, 4, 0),
        
        new juce::AudioParameterInt("pan", "Panning", -100, 100, 0),
        new juce::AudioParameterInt("volume", "Volume", 0, 100, 20)
    });
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{   
    juce::StringArray wavetablePaths;
    for (auto& file : getWavetableFiles()) { wavetablePaths.add(file.getFileName()); }
    
    juce::AudioProcessorValueTreeState::ParameterLayout params {
        std::make_unique<juce::AudioParameterChoice>("wavetablePath", "Wavetable path", wavetablePaths, 1),
        std::make_unique<juce::AudioParameterInt>("polyphony", "Number of voices", 1, 32, 5),
        
        std::make_unique<juce::AudioParameterFloat>("attack", "ADSR envelope attack", 0.0f, 1.0f, 0.1f),
        std::make_unique<juce::AudioParameterFloat>("hold", "ADSR envelope hold", 0.0f, 1.0f, 0.1f),
        std::make_unique<juce::AudioParameterFloat>("decay", "ADSR envelope decay", 0.0f, 1.0f, 0.1f),
        std::make_unique<juce::AudioParameterFloat>("sustain", "ADSR envelope sustain", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("release", "ADSR envelope release", 0.0f, 1.0f, 0.1f),
        
        std::make_unique<juce::AudioParameterFloat>("attackTension", "ADSR envelope attack tension", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>("decayTension", "ADSR envelope decay tension", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>("releaseTension", "ADSR envelope release tension", 0.0f, 1.0f, 0.5f)
    };

    for (auto& param : getModulableParameters())
    {
        params.add((std::unique_ptr<juce::RangedAudioParameter>) param);
    }
    
    for (int i = 0; i < _numberOfAHDSRControllers; i++)
    {
        for (auto& param : getModulableParameters())
        {
            params.add(std::make_unique<juce::AudioParameterFloat>(
                juce::String("AHDSR" + juce::String(i) + "-" + param->paramID),
                juce::String("AHDSR" + juce::String(i) + "-" + param->name),
                juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f),
                0
            ));
        }
    }

    return params;
}
