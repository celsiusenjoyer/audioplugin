#pragma once

#include "ProcessorBase.h"
#include "OscillatorProcessor.h"

class Voice : public ProcessorBase,
              public juce::MPESynthesiserVoice
{
public:
   Voice(juce::AudioProcessorValueTreeState&);
   void prepare(const juce::dsp::ProcessSpec&);
   void noteStarted() override;
   void notePitchbendChanged() override;
   void noteStopped(bool) override;
   void notePressureChanged() override;
   void noteTimbreChanged() override;
   void noteKeyStateChanged() override;
   void renderNextBlock (juce::AudioBuffer<float>&, int, int) override;
private:

   juce::HeapBlock<char> heapBlock;
   juce::dsp::AudioBlock<float> tempBlock;
    
   enum
   {
        osc1Index,
        osc2Index,
        filterIndex,
        masterGainIndex
   };
    
   juce::AudioProcessorValueTreeState& valueTreeState;

   juce::AudioParameterChoice* osc1Waveform;
   juce::AudioParameterChoice* osc2Waveform;
   juce::AudioParameterBool*   osc1Enabled;
   juce::AudioParameterBool*   osc2Enabled;

   juce::dsp::ProcessorChain<OscillatorProcessor<float>, OscillatorProcessor<float>,
                             juce::dsp::LadderFilter<float>, juce::dsp::Gain<float>> processorChain;
    
   static constexpr size_t lfoUpdateRate = 100;
   size_t lfoUpdateCounter = lfoUpdateRate;
   juce::dsp::Oscillator<float> lfo; 
    
};
