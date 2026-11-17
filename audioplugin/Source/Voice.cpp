#include "Voice.h"

Voice::Voice(juce::AudioProcessorValueTreeState& vts) 
        : valueTreeState(vts)
{
    auto& masterGain = processorChain.get<masterGainIndex>();
    masterGain.setGainLinear(valueTreeState.getRawParameterValue("gain")->load());
    
    osc1Waveform = dynamic_cast<juce::AudioParameterChoice*>(valueTreeState.getParameter("osc1Waveform"));
    osc2Waveform = dynamic_cast<juce::AudioParameterChoice*>(valueTreeState.getParameter("osc2Waveform"));


    processorChain.get<osc1Index>().setWaveform(osc1Waveform);
    processorChain.get<osc2Index>().setWaveform(osc2Waveform);

    auto& filter = processorChain.get<filterIndex>();
    filter.setCutoffFrequencyHz (100.0f);
    filter.setResonance(0.7f);
    
    lfo.initialise ([] (float x) { return std::sin(x); }, 128);
    lfo.setFrequency (3.0f);
}

void Voice::prepare(const juce::dsp::ProcessSpec& spec)
{
    tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
    processorChain.prepare(spec);

    lfo.prepare ({ spec.sampleRate / lfoUpdateRate, spec.maximumBlockSize, spec.numChannels });
}

void Voice::noteStarted()
{
    auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
    auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();

    processorChain.get<osc1Index>().setFrequency (freqHz, true);
    processorChain.get<osc1Index>().setLevel (velocity);

    processorChain.get<osc2Index>().setFrequency (freqHz * 1.01f, true);
    processorChain.get<osc2Index>().setLevel(velocity);
}

void Voice::notePitchbendChanged()
{
    auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();
    processorChain.get<osc1Index>().setFrequency(freqHz, false);
    processorChain.get<osc2Index>().setFrequency(freqHz * 1.01f, false);
}

void Voice::noteStopped(bool)
{
    clearCurrentNote();
}

void Voice::notePressureChanged() {}
void Voice::noteTimbreChanged() {}
void Voice::noteKeyStateChanged() {}

void Voice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    auto output = tempBlock.getSubBlock (0, (size_t) numSamples);
    output.clear();
    
    for (size_t pos = 0; pos < (size_t) numSamples;)
    {
        auto max = juce::jmin((size_t) numSamples - pos, lfoUpdateCounter);
        auto block = output.getSubBlock (pos, max);

        juce::dsp::ProcessContextReplacing<float> context (block);
        processorChain.process (context);

        pos += max;
        lfoUpdateCounter -= max;

        if(lfoUpdateCounter == 0)
        {
            lfoUpdateCounter = lfoUpdateRate;
            auto lfoOut = lfo.processSample (0.0f);
            auto curoffFreqHz = juce::jmap (lfoOut, -1.0f, 1.0f, 100.0f, 2000.0f);
            processorChain.get<filterIndex>().setCutoffFrequencyHz (curoffFreqHz);
        }
    }

    juce::dsp::AudioBlock<float> (outputBuffer)
        .getSubBlock ((size_t) startSample, (size_t) numSamples)
        .add(tempBlock);
}

