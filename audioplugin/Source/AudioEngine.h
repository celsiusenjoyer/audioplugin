#pragma once

#include "ProcessorBase.h"
#include "Voice.h"

class AudioEngine : public ProcessorBase,
                        public juce::MPESynthesiser
{
public:

    AudioEngine(juce::AudioProcessorValueTreeState&);
    static constexpr int maxNumVoices = 16; 

    void prepare (const juce::dsp::ProcessSpec&) noexcept;
    const juce::String getName() const override;
private:
    void renderNextSubBlock(juce::AudioBuffer<float>&, int, int) override;
    
    juce::AudioProcessorValueTreeState& valueTreeState;

    enum 
    {
        reverbIndex
    };

    juce::dsp::ProcessorChain<juce::dsp::Reverb> fxChain;
};


