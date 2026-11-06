#pragma once

#include "ProcessorBase.h"

class GainProcessor : public ProcessorBase
{
public:
    GainProcessor();
    const juce::String getName() const override;
    void prepareToPlay (double, int) override;
    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;
    void reset() override;
private:
    juce::dsp::Gain<float> gain;

};
