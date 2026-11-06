#pragma once

#include "ProcessorBase.h"

class FilterProcessor : public ProcessorBase
{
public:
    FilterProcessor();

    const juce::String getName() const override;
    void prepareToPlay(double, int) override;
    void processBlock(juce::AudioSampleBuffer&, juce::MidiBuffer&) override;
private:

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
juce::dsp::IIR::Coefficients<float>> filter;

};
