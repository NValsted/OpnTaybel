#pragma once

#include <JuceHeader.h>
#include "../BaseTable.h"

class AHDSREnvelope : public BaseTable
{
public:
    AHDSREnvelope(juce::AudioProcessorValueTreeState&);
    using BaseTable::getSample;
    float getTableDelta();
    float wrapTableIndex(float tableIndex, bool noteOn);
    void updateEnvelope();
    void prepareToPlay(double sampleRate);

private:
    float sampleRate = 0.0f, tableDelta = 0.0f, sustainIndex = 0.0f, timeSpan = 0.0f;
    juce::AudioProcessorValueTreeState& parameters;
};