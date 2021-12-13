#pragma once

#include <JuceHeader.h>
#include "../Parameters.h"

class WavetableOscillator
{
public:
    WavetableOscillator(juce::AudioProcessorValueTreeState&);
    ~WavetableOscillator();

    void selectWaveTable();
    void prepareToPlay(double sampleRate);
    float getTableDelta(float frequency);
    float getSample(float tableIdx);
    float wrapTableIndex(float tableIdx);        
    void updateTableOffset(int tableState);
    const float* getActiveTableSlice();

    juce::Range<float> amplitudeRange = { -1.0f, 1.0f };
    const float tableSize = (float)_tableSize;  // 2048 samples per state
    juce::String wavetableName;

private:
    juce::AudioProcessorValueTreeState& parameters;
    
    void initWavetable();
    bool verifySample(float sample);
    bool verifyWavetable(juce::AudioSampleBuffer* wavetable);
    void loadWavetableFromFile(juce::File file);

    std::unordered_map<juce::String, int> wavetableMap;
    const float* wavetableRPtr;
    float sampleRate, tableSizeOverSampleRate;
    int tableOffset = 0;
    int maxTableState = 0;
    juce::AudioSampleBuffer wavetable;

    juce::AudioSampleBuffer fileBuffer;
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::AudioFormatManager formatManager;
};
