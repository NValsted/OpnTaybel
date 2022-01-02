#include "AHDSR.h"

AHDSREnvelope::AHDSREnvelope(juce::AudioProcessorValueTreeState& params) : BaseTable(),
                                                                           parameters(params)
{
}

void AHDSREnvelope::prepareToPlay(double sampleRate)
{
    this->sampleRate = (float)sampleRate;
}

float AHDSREnvelope::getTableDelta()
{
    return tableSize / (timeSpan * sampleRate);
}

float AHDSREnvelope::wrapTableIndex(float tableIndex, bool noteOn)
{
    // TODO : if releasing note before attack phase is finished, make sure to compensate for corresponding y-value at time of release.
    float newIndex;
    
    if (noteOn) { newIndex = juce::jmin<float>(tableIndex, sustainIndex); }
    else { newIndex = juce::jmax<float>(tableIndex, sustainIndex);}
    
    return juce::jmin<float>(juce::jmax<float>(newIndex, 0.0f), tableSize);
}

void AHDSREnvelope::updateEnvelope()
{
    timeSpan = parameters.getParameter("attack")->getValue()
    + parameters.getParameter("hold")->getValue()
    + parameters.getParameter("decay")->getValue()
    + parameters.getParameter("release")->getValue();
    
    float attackIndex = 0.0f;
    float holdIndex = parameters.getParameter("attack")->getValue() / timeSpan * tableSize;
    float decayIndex = parameters.getParameter("hold")->getValue() / timeSpan * tableSize + holdIndex;
    sustainIndex = parameters.getParameter("decay")->getValue() / timeSpan * tableSize + decayIndex;
    float releaseIndex = parameters.getParameter("release")->getValue() / timeSpan * tableSize + sustainIndex;

    float sustainLevel = parameters.getParameter("sustain")->getValue();

    float* writePointer = tmpTable.getWritePointer(0);
    for (int i = 0; i < tableSize; i++)
    {
        if (i < holdIndex) { writePointer[i] = (1.0f / holdIndex) * i; }
        else if (i < decayIndex) { writePointer[i] = 1.0f; }
        else if (i < sustainIndex)
        {
            writePointer[i] = 1.0f + (
                (sustainLevel - 1.0f) / (sustainIndex - decayIndex)
                * (i - decayIndex)
            );
        }
        else if (i < releaseIndex)
        {
            writePointer[i] = sustainLevel + (
                (-sustainLevel) / (releaseIndex - sustainIndex)
                * (i - sustainIndex)
            );
        }
        else { writePointer[i] = 0.0f; }
    }
    
    table = tmpTable;
    tableReadPtr = table.getReadPointer(0);
    tmpTable = juce::AudioBuffer<float>(1, tableSize);
}
