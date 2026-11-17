#include "OscillatorProcessor.h"

template <typename Type>
OscillatorProcessor<Type>::OscillatorProcessor() 
{
    setFrequency(440.0f, false);
}

template <typename Type>
void OscillatorProcessor<Type>::setWaveform(juce::AudioParameterChoice* waveform)
{
    auto& osc = processorChain.template get<oscIndex>();
    
    if(waveform->getIndex() == 0) // Sine wave 
    {
        osc.initialise([] (Type x) { return std::sin (x); }, 128);
    }
    else if(waveform->getIndex() == 1) // Saw tooth
    {
        osc.initialise ([] (Type x) {
            return juce::jmap (x,
                Type (-juce::MathConstants<double>::pi),
                Type (juce::MathConstants<double>::pi),
                Type (-1),
                Type (1));
        },
            2);
    }

}

template <typename Type>
void OscillatorProcessor<Type>::prepare (const juce::dsp::ProcessSpec& spec)
{
    processorChain.prepare (spec); 
}

template <typename Type>
void OscillatorProcessor<Type>::setFrequency(Type newValue, bool force)
{
    auto& osc = processorChain.template get<oscIndex>();
    osc.setFrequency(newValue, force);
}

template <typename Type>
void OscillatorProcessor<Type>::setLevel(Type newValue)
{
    auto& gain = processorChain.template get<gainIndex>();
    gain.setGainLinear (newValue);
}

template <typename Type>
template <typename ProcessContext>
void OscillatorProcessor<Type>::process(const ProcessContext& context)
{
    processorChain.process(context);
}

template<typename Type>
void OscillatorProcessor<Type>::reset(){
    processorChain.reset();
}
