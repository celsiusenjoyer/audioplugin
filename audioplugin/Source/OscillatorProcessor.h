#pragma once

#include "ProcessorBase.h"

#include <cmath>

template <typename Type>
class OscillatorProcessor : public ProcessorBase
{
public:
    OscillatorProcessor();
    void setWaveform(juce::AudioParameterChoice*);
    void reset() noexcept;
    void prepareToPlay(double, int) override;
    void prepare(const juce::dsp::ProcessSpec&);
    const juce::String getName() const override;
    void setFrequency(Type, bool);
    void setLevel(Type);
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    template <typename ProcessContext>
    void process(const ProcessContext&) noexcept;
private:

    enum {
        oscIndex,
        gainIndex
    };
    
    juce::dsp::ProcessorChain<juce::dsp::Oscillator<Type>, juce::dsp::Gain<Type>> processorChain;

    float previousGain;
};

#include "OscillatorProcessor.tpp"
