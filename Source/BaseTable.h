#pragma once

#include <JuceHeader.h>
#include "Parameters.h"

class BaseTable
{
public:
    BaseTable();
    ~BaseTable();

    const float tableSize = (float)_tableSize;

    void prepareToPlay(double sampleRate);
    float getTableDelta(float frequency);
    float getSample(float tableIndex);
    float wrapTableIndex(float tableIndex);
    void updateTableOffset(int tableState);
    juce::Range<float> getTableRange();

protected:
    const float* tableReadPtr;
    float sampleRate = 0.0f, tableSizeOverSampleRate = 0.0f;
    int tableOffset = 0;
    int maxTableState = 0;
    juce::AudioSampleBuffer table;
    juce::AudioSampleBuffer tmpTable;  // Used when updating table
};
