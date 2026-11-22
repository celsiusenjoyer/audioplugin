#pragma once

#include "ProcessorBase.h"
#include "Voice.h"

class AudioEngine : public ProcessorBase,
                        public juce::MPESynthesiser
{
public:

    AudioEngine(juce::AudioProcessorValueTreeState&);
    static constexpr int maxNumVoices = 4; 
    void prepareToPlay(double, int) override;
    void prepare (const juce::dsp::ProcessSpec&) noexcept;
    const juce::String getName() const override;
    void processBlock(juce::AudioSampleBuffer&, juce::MidiBuffer&) override;
    bool acceptsMidi() const override;
private:
    void renderNextSubBlock(juce::AudioBuffer<float>&, int, int) override;
    
    juce::AudioProcessorValueTreeState& valueTreeState;

    enum 
    {
        reverbIndex
    };

    juce::dsp::ProcessorChain<juce::dsp::Gain> fxChain;
};


