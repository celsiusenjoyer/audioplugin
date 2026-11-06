#include "GainProcessor.h"

GainProcessor::GainProcessor(){
    gain.setGainDecibels (-6.0f);
}

const juce::String GainProcessor::getName() const { return "Gain"; }

void GainProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) 
{
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), 2 };
    gain.prepare (spec);
}

void GainProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    gain.process (context);
}

void GainProcessor::reset(){
    gain.reset();
}


