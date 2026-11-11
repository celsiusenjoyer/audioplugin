#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
using Node = juce::AudioProcessorGraph::Node;

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor,
                                        private juce::Timer

{
public:
    float noteOnVel;
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void initialiseGraph();
    void updateGraph();
    void connectAudioNodes();
    void connectMidiNodes();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    void timerCallback() override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;


    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::Random random;
    
    juce::StringArray processorChoices { "Empty", "Oscillator", "Gain", "Filter" };
    std::unique_ptr<juce::AudioProcessorGraph> mainProcessor;

    std::atomic<float>* phaseParameter = nullptr;
    std::atomic<float>* gainParameter = nullptr;
    
    juce::AudioParameterBool* muteInput = nullptr;

    juce::AudioParameterChoice* fxSlot1 = nullptr;
    juce::AudioParameterChoice* fxSlot2 = nullptr;
    juce::AudioParameterChoice* fxSlot3 = nullptr;

    juce::AudioParameterBool* fxBypass1 = nullptr;
    juce::AudioParameterBool* fxBypass2 = nullptr;
    juce::AudioParameterBool* fxBypass3 = nullptr;

    Node::Ptr audioInputNode;
    Node::Ptr audioOutputNode;
    Node::Ptr midiInputNode;
    Node::Ptr midiOutputNode;

    Node::Ptr slot1Node;
    Node::Ptr slot2Node;
    Node::Ptr slot3Node;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState parameters;

    float previousGain;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
