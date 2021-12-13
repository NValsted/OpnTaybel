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

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{   
    juce::StringArray wavetablePaths;
    for (auto& file : getWavetableFiles()) { wavetablePaths.add(file.getFileName()); }
    
    juce::AudioProcessorValueTreeState::ParameterLayout params {
        std::make_unique<juce::AudioParameterInt>("tableState", "Wavetable state", 0, 255, 0),
        std::make_unique<juce::AudioParameterChoice>("wavetablePath", "Wavetable path", wavetablePaths, 1),
        std::make_unique<juce::AudioParameterFloat>("tablePhase", "Wavetable phase offset", juce::NormalisableRange<float>(0.0f, (float)_tableSize), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("tablePhaseRand", "Wavetable phase randomization", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f),
        std::make_unique<juce::AudioParameterInt>("unisonVoices", "Unison voices", 1, 16, 1),
        std::make_unique<juce::AudioParameterFloat>("unisonDetune", "Unison detune", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.5f), 0.25f),
        std::make_unique<juce::AudioParameterInt>("unisonDetuneSemitone", "Max unison detune in semitones", 1, 24, 1),
        std::make_unique<juce::AudioParameterFloat>("unisonBlend", "Unison blend", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("unisonStereo", "Unison stereo width", juce::NormalisableRange<float>(0.0f, (float)_tableSize / 4.0f, 1.0f, 0.5f), 0.0f),
        std::make_unique<juce::AudioParameterFloat>("pitchFine", "Fine pitch offset", -1.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("pitchCoarse", "Coarse pitch offset", -64.0f, 64.0f, 0.0f),
        std::make_unique<juce::AudioParameterInt>("pitchSemitone", "Pitch semitone offset", -12, 12, 0),
        std::make_unique<juce::AudioParameterInt>("pitchOctave", "Pitch octave offset", -4, 4, 0),
        std::make_unique<juce::AudioParameterInt>("pan", "Panning", -100, 100, 0),
        std::make_unique<juce::AudioParameterInt>("volume", "Volume", 0, 100, 20)
    };
    return params;
}
