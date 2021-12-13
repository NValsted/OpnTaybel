#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

OpnTaybelAudioProcessor::OpnTaybelAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
                    #if ! JucePlugin_IsMidiEffect
                        #if ! JucePlugin_IsSynth
                            .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                        #endif
                            .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                    #endif
                    ),
#endif
    parameters(
        *this, nullptr, juce::Identifier("OpnTaybelParameters"), createParameterLayout()
    ),
    keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    synth(parameters)
{
    for (int i = 0; i < 4; ++i) {
        synth.addVoice(new WavetableVoice(&synth));
    }
    synth.addSound(new WavetableSound());
}

OpnTaybelAudioProcessor::~OpnTaybelAudioProcessor()
{
}

const juce::String OpnTaybelAudioProcessor::getName() const
{
    #if defined(JucePlugin_Name)
        return JucePlugin_Name;
    #endif
}

bool OpnTaybelAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OpnTaybelAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OpnTaybelAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OpnTaybelAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OpnTaybelAudioProcessor::getNumPrograms()
{
    return 1;
}

int OpnTaybelAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OpnTaybelAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OpnTaybelAudioProcessor::getProgramName (int index)
{
    return {};
}

void OpnTaybelAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void OpnTaybelAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    synth.getWavetableController(0)->getOscillator()->prepareToPlay(sampleRate);  // TODO: Should probably call preparyToPlay on synth and have it call this on its children 
}

void OpnTaybelAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OpnTaybelAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffects
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    
  #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
  #endif

    return true;
  #endif
}
#endif

void OpnTaybelAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    int totalNumInputChannels  = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();

    int numSamples = buffer.getNumSamples();
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, numSamples);
    }
    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
    synth.renderNextBlock(buffer, midiMessages, 0, numSamples);
}

bool OpnTaybelAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* OpnTaybelAudioProcessor::createEditor()
{
    return new OpnTaybelAudioProcessorEditor(*this, parameters);
}

void OpnTaybelAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ValueTree state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void OpnTaybelAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(parameters.state.getType()))
    {
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OpnTaybelAudioProcessor();
}
