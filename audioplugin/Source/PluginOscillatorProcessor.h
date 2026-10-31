#pragma once

#include "PluginProcessorBase.h"

#include <juce_audio_processors/juce_audio_processors.h>

#include <cmath>

class PluginOscillatorProcessor : PluginProcessorBase
{
public:
    const juce::String getName() const override; 
private:
    juce::dsp::Oscillator<float> oscillator;

};
