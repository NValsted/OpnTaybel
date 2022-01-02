#include "BaseTable.h"

BaseTable::BaseTable() : table(1, _tableSize), tmpTable(1, _tableSize)
{
    table.clear();
    tmpTable.clear();
    tableReadPtr = table.getReadPointer(0);
}

BaseTable::~BaseTable()
{
}

void BaseTable::prepareToPlay(double sampleRate)
{
    this->sampleRate = (float)sampleRate;
    tableSizeOverSampleRate = tableSize / sampleRate;
}

float BaseTable::getTableDelta(float frequency)
{
    return frequency * tableSizeOverSampleRate;
}

float BaseTable::getSample(float tableIndex)
{
    // TODO : do modulo instead of wrapTableIndex
    int intTableIndex = (int)tableIndex + tableOffset;
    float t0 = (tableIndex + (float)tableOffset) - (float)intTableIndex;
    float t1 = 1 - t0;
    return tableReadPtr[intTableIndex] * t1 + tableReadPtr[intTableIndex + 1] * t0;
}

float BaseTable::wrapTableIndex(float tableIndex)
{
    if (tableIndex < tableSize) { return tableIndex; }
    return tableIndex - tableSize;
}

void BaseTable::updateTableOffset(int tableState)
{
    int tmp = std::roundf(juce::jmap((float)tableState, 0.0f, 255.0f, 0.0f, (float)maxTableState));
    tableOffset = tmp * (tableSize + 1);
}

juce::Range<float> BaseTable::getTableRange()
{
    return table.findMinMax(0, 0, table.getNumSamples());
}
