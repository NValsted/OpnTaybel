#include "PluginProcessor.h"
#include "PluginEditor.h"

OpnTaybelAudioProcessorEditor::OpnTaybelAudioProcessorEditor(OpnTaybelAudioProcessor& parent,
                                                             juce::AudioProcessorValueTreeState& params) : AudioProcessorEditor(&parent),
                                                                                                           audioProcessor(parent),
                                                                                                           parameters(params)
{
    wavetableController = audioProcessor.synth.getWavetableController(0);
    adsrController = audioProcessor.synth.getADSRController(0);
    
    addAndMakeVisible(audioProcessor.keyboardComponent);
    addAndMakeVisible(modWheel);
    addAndMakeVisible(pitchWheel);
    addAndMakeVisible(wavetableController);
    addAndMakeVisible(adsrController);

    setSize(800, 600);
}

OpnTaybelAudioProcessorEditor::~OpnTaybelAudioProcessorEditor()
{
}

void OpnTaybelAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void OpnTaybelAudioProcessorEditor::resized()
{
    int width = getWidth();
    int height = getHeight();
    
    int kbcWidth = (int)(width * 0.9);
    int kbcHeight = (int)(height * 0.2);
    audioProcessor.keyboardComponent.setBounds(
        width - (kbcWidth + 10),
        height - (kbcHeight + 10),
        kbcWidth,
        kbcHeight
    );

    modWheel.setBounds(10, height - (kbcHeight + 10), 20, kbcHeight);
    pitchWheel.setBounds(40, height - (kbcHeight + 10), 20, kbcHeight);
    wavetableController->setBounds(10, 10, 400, 150);
    adsrController->setBounds(10, wavetableController->getBottom() + 10, 400, 150);
}
