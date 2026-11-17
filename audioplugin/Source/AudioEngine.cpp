#include "AudioEngine.h"

AudioEngine::AudioEngine(juce::AudioProcessorValueTreeState& vts)
    : valueTreeState(vts)
{
    for (auto i = 0; i < maxNumVoices; ++i)
        addVoice(new Voice(valueTreeState)); 
    
    setVoiceStealingEnabled(true);
}

const juce::String AudioEngine::getName() const 
{
    return { "Reverb" };
}

void AudioEngine::prepare (const juce::dsp::ProcessSpec& spec) noexcept
{
    setCurrentPlaybackSampleRate (spec.sampleRate);

    for(auto* v : voices)
        dynamic_cast<Voice*> (v)->prepare(spec);

    fxChain.prepare(spec);
}

void AudioEngine::renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples)
{
    MPESynthesiser::renderNextSubBlock(outputAudio, startSample, numSamples);

    auto block = juce::dsp::AudioBlock<float> (outputAudio);
    auto blockToUse = block.getSubBlock ((size_t) startSample, (size_t) numSamples);
    auto contextToUse = juce::dsp::ProcessContextReplacing<float> (blockToUse);
    fxChain.process(contextToUse);
}
