#pragma once

#include "AudioEngine.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_devices/juce_audio_devices.h>

using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
using Node = juce::AudioProcessorGraph::Node;

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor

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

    juce::MidiMessageCollector& getMidiMessageCollector() noexcept;

private:
    //==============================================================================
    juce::Random random;
    
    juce::StringArray processorChoices { "Empty", "Oscillator", "Gain", "Filter" };
    juce::StringArray oscillatorWaveformChoices { "sinewave", "sawtooth" };
    std::unique_ptr<juce::AudioProcessorGraph> mainProcessor;

    std::atomic<float>* phase = nullptr;
    
    // Gain
    std::atomic<float>* gain = nullptr;

    float previousGain;

    // Mute
    juce::AudioParameterBool* muteInput = nullptr;
    
    // FX Slots
    juce::AudioParameterChoice* fxSlot1 = nullptr;
    juce::AudioParameterChoice* fxSlot2 = nullptr;
    juce::AudioParameterChoice* fxSlot3 = nullptr;
    
    // Bypass Slots
    juce::AudioParameterBool* fxBypass1 = nullptr;
    juce::AudioParameterBool* fxBypass2 = nullptr;
    juce::AudioParameterBool* fxBypass3 = nullptr;
    
    // Oscillator Waveforms
    juce::AudioParameterChoice* osc1Waveform = nullptr;
    juce::AudioParameterChoice* osc2Waveform = nullptr;
    
    Node::Ptr audioInputNode;
    Node::Ptr audioOutputNode;
    Node::Ptr midiInputNode;
    Node::Ptr midiOutputNode;

    Node::Ptr slot1Node;
    Node::Ptr slot2Node;
    Node::Ptr slot3Node;

    Node::Ptr audioEngineNode; 

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState parameters;
    
    AudioEngine audioEngine{parameters};
    juce::MidiMessageCollector midiMessageCollector;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};

