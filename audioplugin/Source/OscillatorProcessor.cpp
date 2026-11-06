#include "OscillatorProcessor.h"
OscillatorProcessor::OscillatorProcessor()
{
    oscillator.setFrequency (440.0f);
    oscillator.initialise([] (float x) { return std::sin (x); });
}
const juce::String OscillatorProcessor::getName() const
{ return "Oscillator"; }

void OscillatorProcessor::prepareToPlay(double sampleRate, int samplesPerBlock){
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), 2 };
    oscillator.prepare(spec);
}

void OscillatorProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&){
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    oscillator.process(context);
}

void OscillatorProcessor::reset(){
    oscillator.reset();
}

