#include "ReverbProcessor.h"

ReverbProcessor::ReverbProcessor(juce::AudioProcessorValueTreeState& vts)
    : valueTreeState(vts)
{
    for (auto i = 0; i < maxNumVoices; ++i)
        addVoice(new Voice(valueTreeState)); 
    
    setVoiceStealingEnabled(true);
}

const juce::String ReverbProcessor::getName() const 
{
    return { "Reverb" };
}

void ReverbProcessor::prepare (const juce::dsp::ProcessSpec& spec) noexcept
{
    setCurrentPlaybackSampleRate (spec.sampleRate);

    for(auto* v : voices)
        dynamic_cast<Voice*> (v)->prepare(spec);

    reverbChain.prepare(spec);
}

void ReverbProcessor::renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples)
{
    MPESynthesiser::renderNextSubBlock(outputAudio, startSample, numSamples);

    auto block = juce::dsp::AudioBlock<float> (outputAudio);
    auto blockToUse = block.getSubBlock ((size_t) startSample, (size_t) numSamples);
    auto contextToUse = juce::dsp::ProcessContextReplacing<float> (blockToUse);
    reverbChain.process(contextToUse);
}
