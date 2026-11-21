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
    return { "AudioEngine" };
}

void AudioEngine::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), 2 };
    prepare(spec);
}

void AudioEngine::prepare (const juce::dsp::ProcessSpec& spec) noexcept
{
    setCurrentPlaybackSampleRate (spec.sampleRate);

    for(auto* v : voices)
        dynamic_cast<Voice*> (v)->prepare(spec);

    fxChain.prepare(spec);
}

void AudioEngine::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiBuffer)
{
    renderNextBlock (buffer, midiBuffer, 0, buffer.getNumSamples());
}

void AudioEngine::renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples)
{
    MPESynthesiser::renderNextSubBlock(outputAudio, startSample, numSamples);

    auto block = juce::dsp::AudioBlock<float> (outputAudio);
    auto blockToUse = block.getSubBlock ((size_t) startSample, (size_t) numSamples);
    auto contextToUse = juce::dsp::ProcessContextReplacing<float> (blockToUse);
    fxChain.process(contextToUse);
}

bool AudioEngine::acceptsMidi() const
{
    return true;
}
