#include "PluginProcessorBase.h"


PluginProcessorBase::PluginProcessorBase()
        : AudioProcessor (BusesProperties().withInput ("Input", juce::AudioChannelSet::stereo()).withOutput ("Output", juce::AudioChannelSet::stereo()))
{

}
