#pragma once

#include <JuceHeader.h>

#define _baseDirectory juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("OpnTaybel")
#define _settingsDirectory _baseDirectory.getChildFile(".settings")
#define _tablesDirectory _baseDirectory.getChildFile("Tables")
#define _presetsDirectory _baseDirectory.getChildFile("Presets")

#define _tableSize 2048
#define _numberOfOscillators 4

juce::Array<juce::File> getWavetableFiles();
std::unordered_map<juce::String, int> getWavetableMap();
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
