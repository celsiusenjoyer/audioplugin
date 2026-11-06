#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class ProcessorBase : public juce::AudioProcessor
{
public:
    //==============================================================================
    ProcessorBase();
    //==============================================================================
    void prepareToPlay (double, int) override;
    void releaseResources() override;
    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override; 
    bool hasEditor() const override;
    //==============================================================================
    const juce::String getName() const override; 
    bool acceptsMidi() const override; 
    bool producesMidi() const override; 
    double getTailLengthSeconds() const override; 
    //==============================================================================
    int getNumPrograms() override; 
    int getCurrentProgram() override;
    void setCurrentProgram (int) override; 
    const juce::String getProgramName (int) override; 
    void changeProgramName (int, const juce::String&) override; 
    //==============================================================================
    void getStateInformation (juce::MemoryBlock&) override; 
    void setStateInformation (const void*, int) override; 
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorBase)
};
