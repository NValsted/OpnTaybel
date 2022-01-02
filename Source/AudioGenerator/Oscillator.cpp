#include "Oscillator.h"

WavetableOscillator::WavetableOscillator(juce::AudioProcessorValueTreeState& params) : parameters(params)
{
    // TODO: inherit from BaseTable
    formatManager.registerBasicFormats();
    wavetableMap = getWavetableMap();
    loadWavetableFromFile(
        _tablesDirectory.getChildFile(parameters.getParameter("wavetablePath")->getCurrentValueAsText())
    );
    
}

WavetableOscillator::~WavetableOscillator()
{
}

void WavetableOscillator::prepareToPlay(double sampleRate)
{
    this->sampleRate = (float)sampleRate;
    tableSizeOverSampleRate = tableSize / sampleRate;
}

float WavetableOscillator::getTableDelta(float frequency)
{
    return frequency * tableSizeOverSampleRate;
}

float WavetableOscillator::getSample(float tableIdx)
{
    // TODO : do modulo instead of wrapTableIndex
    int intTableIdx = (int)tableIdx + tableOffset;
    float t0 = (tableIdx + (float)tableOffset) - (float)intTableIdx;
    float t1 = 1 - t0;
    return wavetableRPtr[intTableIdx] * t1 + wavetableRPtr[intTableIdx + 1] * t0;
}

float WavetableOscillator::wrapTableIndex(float tableIdx)
{
    if (tableIdx < tableSize) { return tableIdx; }
    return tableIdx - tableSize;
}

void WavetableOscillator::updateTableOffset(int tableState)
{
    int tmp = std::roundf(juce::jmap((float)tableState, 0.0f, 255.0f, 0.0f, (float)maxTableState));
    tableOffset = tmp * (tableSize + 1);
}

const float* WavetableOscillator::getActiveTableSlice()
{
    return &wavetableRPtr[tableOffset];
}

bool WavetableOscillator::verifySample(float sample)
{
    return (sample >= -1.0f && sample <= 1.0f);
};

bool WavetableOscillator::verifyWavetable(juce::AudioSampleBuffer* wavetable)
{
    const float* readPointer = wavetable->getReadPointer(0);
    for (int i = 0; i < wavetable->getNumSamples(); i++)
    {
        if (!verifySample(readPointer[i]))
        {
            juce::String errorMessage = "Sample " + juce::String(i) + " is not in the range of [-1, 1]";
            juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::WarningIcon,
                "Invalid Sample " + std::to_string(readPointer[i]),
                errorMessage
            );
            return false;
        }
    }
    return true;
}

void WavetableOscillator::selectWaveTable()
{
    fileChooser = std::make_unique<juce::FileChooser>(
        "Select wavetable",
        _tablesDirectory,
        "*.wav"
    );
    int chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    fileChooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc){
        juce::File file = fc.getResult();
        loadWavetableFromFile(file);
    });
}

void WavetableOscillator::loadWavetableFromFile(juce::File file)
{
    if (file == juce::File{}) { return; }

    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader.get() != nullptr) {
        int numberSamples = (int)reader->lengthInSamples;
        fileBuffer.setSize((int)reader->numChannels, numberSamples);
        reader->read(&fileBuffer, 0, numberSamples, 0, true, true);

        /*if (!verifyWavetable(&fileBuffer)) {
            return;
        }*/

        wavetableName = file.getFileName().replace(".wav", "");
        float mappedWavetable = (float)wavetableMap[file.getFileName()] / wavetableMap.size();  // TODO : it is problematic that the wavetablePath parameter has to be set with a float, as it is dependent on having the same tables present across all machines that have to be compatible with a preset.
        parameters.getParameter("wavetablePath")->setValueNotifyingHost(mappedWavetable);

        maxTableState = (int)std::truncf((float)numberSamples / tableSize - 1.0f);
        wavetable.setSize((int)reader->numChannels, numberSamples + maxTableState + 1);
        // Inject a sample into each tableState of tableSize number of samples with the last sample being a duplicate of the first.
        const float* inSamples = fileBuffer.getReadPointer(0);
        float* outSamples = wavetable.getWritePointer(0);
        int offset = 0;
        int intTableSize = (int)std::roundf(tableSize);
        
        for (int i = 0; i < numberSamples; i++)
        {
            if (i % intTableSize == 0 && i != 0)
            {
                outSamples[i + offset] = inSamples[i - intTableSize];
                offset++;
            }
            outSamples[i + offset] = inSamples[i];
        }
        outSamples[numberSamples + offset] = inSamples[numberSamples - intTableSize];
        updateTableOffset(parameters.getParameter("tableState")->getCurrentValueAsText().getIntValue());
        wavetableRPtr = wavetable.getReadPointer(0);
    }
}
