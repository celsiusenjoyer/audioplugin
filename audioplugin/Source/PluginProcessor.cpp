#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "OscillatorProcessor.h"
#include "FilterProcessor.h"
#include "GainProcessor.h"
#include <iostream>

juce::AudioProcessorValueTreeState::ParameterLayout
AudioPluginAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    

    layout.add(std::make_unique<juce::AudioParameterFloat> (juce::ParameterID{"gain", 1}, "Gain", juce::NormalisableRange<float> (0.0f, 1.0f), 0.5f)); 
    layout.add(std::make_unique<juce::AudioParameterBool>  (juce::ParameterID{"invertPhase", 1}, "Invert Phase", false));
    layout.add(std::make_unique<juce::AudioParameterBool>  (juce::ParameterID{"muteInput", 1}, "Mute Input", false));

    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"fxSlot1", 1}, "FX Slot 1", processorChoices, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"fxSlot2", 1}, "FX Slot 2", processorChoices, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"fxSlot3", 1}, "FX Slot 3", processorChoices, 0));

    layout.add(std::make_unique<juce::AudioParameterBool>  (juce::ParameterID{"fxBypass1", 1}, "FX Bypass 1", false));
    layout.add(std::make_unique<juce::AudioParameterBool>  (juce::ParameterID{"fxBypass2", 1}, "FX Bypass 2", false));
    layout.add(std::make_unique<juce::AudioParameterBool>  (juce::ParameterID{"fxBypass3", 1}, "FX Bypass 3", false));

    return layout;
}
//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       parameters (*this, nullptr, juce::Identifier ("APVTS"), createParameterLayout()),
       mainProcessor(new juce::AudioProcessorGraph())
{

    gainParameter = parameters.getRawParameterValue("gain");
    phaseParameter = parameters.getRawParameterValue("invertPhase");
    /*
    muteInputParameter = parameters.getRawParameterValue("muteInput");
    fxSlot1Parameter = parameters.getRawParameterValue("fxSlot1");
    fxSlot2Parameter = parameters.getRawParameterValue("fxSlot2");
    fxSlot3Parameter = parameters.getRawParameterValue("fxSlot3");
    fxBypass1Parameter = parameters.getRawParameterValue("fxBypass1");
    fxBypass2Parameter = parameters.getRawParameterValue("fxBypass2");
    fxBypass3Parameter = parameters.getRawParameterValue("fxBypass3");
    */
     muteInput = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("muteInput"));
     fxSlot1 = dynamic_cast<juce::AudioParameterChoice*>(parameters.getParameter("fxSlot1"));
     fxSlot2 = dynamic_cast<juce::AudioParameterChoice*>(parameters.getParameter("fxSlot2"));
     fxSlot3 = dynamic_cast<juce::AudioParameterChoice*>(parameters.getParameter("fxSlot3"));
     fxBypass1 = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("fxBypass1"));
     fxBypass2 = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("fxBypass2"));
     fxBypass3 = dynamic_cast<juce::AudioParameterBool*>(parameters.getParameter("fxBypass3"));
     
     startTimer(100);
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::initialiseGraph(){
    mainProcessor->clear();
    audioInputNode = mainProcessor->addNode (std::make_unique<AudioGraphIOProcessor>
(AudioGraphIOProcessor::audioInputNode));
    audioOutputNode = mainProcessor->addNode (std::make_unique<AudioGraphIOProcessor>
(AudioGraphIOProcessor::audioOutputNode));
    midiInputNode = mainProcessor->addNode (std::make_unique<AudioGraphIOProcessor>
(AudioGraphIOProcessor::midiInputNode));
    midiOutputNode = mainProcessor->addNode (std::make_unique<AudioGraphIOProcessor>
(AudioGraphIOProcessor::midiOutputNode));
    connectAudioNodes();
    connectMidiNodes();

}

void AudioPluginAudioProcessor::updateGraph(){
    bool hasChanged = false;

    // Create arrays for parameters for traversal
    juce::Array<juce::AudioParameterChoice*> choices { fxSlot1, fxSlot2, fxSlot3 };
    juce::Array<juce::AudioParameterBool*> fxBypasses { fxBypass1, fxBypass2, fxBypass3 };

    // Add node slots
    juce::ReferenceCountedArray<Node> slots;
    slots.add (slot1Node);
    slots.add (slot2Node);
    slots.add (slot3Node);
    
    for (int i = 0; i < choices.size(); ++i){
        auto& choice = choices.getReference(i);
        auto slot = slots.getUnchecked (i);
        if (choice->getIndex() == 0)
        {
            if (slot != nullptr)
            {
                mainProcessor->removeNode (slot.get());
                slots.set(i, nullptr);
                hasChanged = true;
            }
        }
        else if (choice->getIndex() == 1)
        {
            if (slot != nullptr)
            {
                if (slot->getProcessor()->getName() == "Oscillator")
                    continue;
                mainProcessor->removeNode (slot.get());
            }
            slots.set (i, mainProcessor->addNode (std::make_unique<OscillatorProcessor>()));
            hasChanged = true;
        }
        else if (choice->getIndex() == 2)
        {
            if(slot != nullptr)
            {
                if(slot->getProcessor()->getName() == "Gain")
                    continue;
                mainProcessor->removeNode (slot.get());
            }
            slots.set (i, mainProcessor->addNode (std::make_unique<GainProcessor>()));
            hasChanged = true;
        }
        else if (choice->getIndex() == 2)
        {
            if(slot != nullptr)
            {
                if(slot->getProcessor()->getName() == "Filter")
                    continue;
                mainProcessor->removeNode (slot.get());
            }
            slots.set (i, mainProcessor->addNode (std::make_unique<FilterProcessor>()));
            hasChanged = true;
        }
    }

    if(hasChanged)
    {
        for (auto connection : mainProcessor->getConnections())
            mainProcessor->removeConnection(connection);
        juce::ReferenceCountedArray<Node> activeSlots;
        for (auto slot : slots)
        {
            if (slot != nullptr)
            {
                activeSlots.add (slot);
                slot->getProcessor()->setPlayConfigDetails (getMainBusNumInputChannels(),
                    getMainBusNumOutputChannels(),
                    getSampleRate(),
                    getBlockSize());
            }
        }
        if (activeSlots.isEmpty())
        {
            connectAudioNodes();
        }
        else
        {
            for (int i = 0; i < activeSlots.size() - 1; ++i) 
            {
                for (int channel = 0; channel < 2; ++channel)
                    mainProcessor->addConnection ({ {activeSlots.getUnchecked (i)->nodeID, channel },
                                                  { activeSlots.getUnchecked (i + 1)->nodeID, channel } });
            }
            for (int channel = 0; channel < 2; ++channel) // Link first active slot to audio input node and last active slot to audio output node 
            {
                mainProcessor->addConnection ({ { audioInputNode->nodeID,          channel },
                                                { activeSlots.getFirst()->nodeID,  channel } });
                mainProcessor->addConnection ({ { activeSlots.getLast()->nodeID,   channel },
                                                { audioOutputNode->nodeID,         channel } });
            }
        }
        connectMidiNodes();
        for (auto node : mainProcessor->getNodes())
            node->getProcessor()->enableAllBuses();
    }
    for (int i = 0; i < 3; ++i)
    {
        auto slot = slots.getUnchecked(i);
        auto& bypass = fxBypasses.getReference(i);
        if (slot != nullptr)
            slot->setBypassed (bypass->get());
    }
    audioInputNode->setBypassed (muteInput->get());
    slot1Node = slots.getUnchecked(0);
    slot2Node = slots.getUnchecked(1);
    slot3Node = slots.getUnchecked(2);
}
    


void AudioPluginAudioProcessor::connectAudioNodes(){
    for (int channel = 0; channel < 2; ++channel)
        mainProcessor->addConnection ({ { audioInputNode->nodeID, channel },
            { audioOutputNode->nodeID, channel } });
}

void AudioPluginAudioProcessor::connectMidiNodes(){
    mainProcessor->addConnection ({ { midiInputNode->nodeID,
juce::AudioProcessorGraph::midiChannelIndex },
    { midiOutputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex } });
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    juce::String message;
    message << "Preparing to play audio...\n";
    message << " samplesPerBlock = " << samplesPerBlock << "\n";
    message << " sampleRate = " << sampleRate;
    juce::Logger::getCurrentLogger()->writeToLog (message);

    auto phase = *phaseParameter ? -1.0f : 1.0f;
    previousGain = *gainParameter * phase;

    mainProcessor->setPlayConfigDetails(getMainBusNumInputChannels(),
                                        getMainBusNumOutputChannels(),
                                        sampleRate,
                                        samplesPerBlock);
    mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);
    initialiseGraph();

}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    mainProcessor->releaseResources();
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::disabled())
        return false;
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    updateGraph();

    auto phase = *phaseParameter ? -1.0f : 1.0f;
    auto currentGain = *gainParameter * phase;
    
    if (juce::approximatelyEqual (currentGain, previousGain)){
        buffer.applyGain(currentGain);
    }else{
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.a
    
    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // Get a pointer to the start sample in the buffer for this audio output channel
        auto* channelData = buffer.getWritePointer (channel);
        // Fill the required member of samples with noise between -0.125 and +0.125
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
            channelData[sample] = random.nextFloat() * 0.25f - 0.125f;

        // ..do something to the data...
    }
    */

    /*juce::MidiBuffer processedMidi;
    for(const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;
        if (message.isNoteOn())
        {
            message = juce::MidiMessage::noteOn(message.getChannel(),
                message.getNoteNumber(),
                (juce::uint8) noteOnVel);
        }
        processedMidi.addEvent(message, time);
    }
    midiMessages.swapWith(processedMidi);

    */


    mainProcessor->processBlock(buffer, midiMessages);
}

void AudioPluginAudioProcessor::timerCallback()
{
    updateGraph();
}
//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
