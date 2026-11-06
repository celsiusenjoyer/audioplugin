#pragma once

#include "ProcessorBase.h"

#include <cmath>

class OscillatorProcessor : public ProcessorBase
{
public:
    OscillatorProcessor();
    const juce::String getName() const override; 
    void prepareToPlay (double, int) override;
    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;
    void reset() override;
private:
    juce::dsp::Oscillator<float> oscillator;

};
