#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p,
                                                                  juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{

    // Gain
    gainLabel.setText("Noise level in dB", juce::dontSendNotification);

    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (vts, "gain", gainSlider);

    addAndMakeVisible(gainLabel);
    addAndMakeVisible(gainSlider);

    // Mute input
    muteInputButton.setButtonText("Mute Input");
    muteInputButton.setToggleState(true, juce::dontSendNotification);

    muteInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (vts, "muteInput", muteInputButton);

    addAndMakeVisible(muteInputButton);

    // Osc waveform ComboBox
    osc1WaveformLabel.setText("Waveform", juce::dontSendNotification);
    osc2WaveformLabel.setText("Waveform", juce::dontSendNotification);
    
    addAndMakeVisible(osc1WaveformLabel);
    addAndMakeVisible(osc2WaveformLabel);
    
    osc1WaveformCB.addItemList(waveformChoices, 1);
    osc2WaveformCB.addItemList(waveformChoices, 1);

    osc1WaveformCBAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        (vts, "osc1Waveform", osc1WaveformCB);
    osc2WaveformCBAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        (vts, "osc2Waveform", osc2WaveformCB);

    addAndMakeVisible(osc1WaveformCB);
    addAndMakeVisible(osc2WaveformCB);

    osc1WaveformLabel.attachToComponent(&osc1WaveformCB, true);
    osc2WaveformLabel.attachToComponent(&osc2WaveformCB, true);

    // Osc enable 
    osc1Enabled.setButtonText("Enable");
    osc1Enabled.setToggleState("true", juce::dontSendNotification);
    
    osc1EnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (vts, "osc1Enabled", osc1Enabled);

    osc2Enabled.setButtonText("Enable");
    osc2Enabled.setToggleState("false", juce::dontSendNotification);

    osc2EnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (vts, "osc2Enabled", osc2Enabled);
    
    addAndMakeVisible(osc1Enabled);
    addAndMakeVisible(osc2Enabled);

    // FX Slot Combo box
    fxSlot1Label.setText("FX Slot 1", juce::dontSendNotification);
    fxSlot2Label.setText("FX Slot 2", juce::dontSendNotification);
    fxSlot3Label.setText("FX Slot 3", juce::dontSendNotification);

    addAndMakeVisible(fxSlot1Label);
    addAndMakeVisible(fxSlot2Label);
    addAndMakeVisible(fxSlot3Label);
    
    fxSlot1CB.addItemList(processorChoices, 1);
    fxSlot2CB.addItemList(processorChoices, 1);
    fxSlot3CB.addItemList(processorChoices, 1);

    fxSlot1CBAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        (vts, "fxSlot1", fxSlot1CB);
    fxSlot2CBAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        (vts, "fxSlot2", fxSlot2CB);
    fxSlot3CBAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        (vts, "fxSlot3", fxSlot3CB);

    addAndMakeVisible(fxSlot1CB);
    addAndMakeVisible(fxSlot2CB);
    addAndMakeVisible(fxSlot3CB);

    fxSlot1Label.attachToComponent(&fxSlot1CB, true);
    fxSlot2Label.attachToComponent(&fxSlot2CB, true);
    fxSlot3Label.attachToComponent(&fxSlot3CB, true);
    
    // Bypass buttons
    fxBypassSlot1.setButtonText("FX Bypass 1");
    fxBypassSlot2.setButtonText("FX Bypass 2");
    fxBypassSlot3.setButtonText("FX Bypass 3");
    
    fxBypassSlot1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (vts, "fxBypass1", fxBypassSlot1);
    fxBypassSlot2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (vts, "fxBypass2", fxBypassSlot2);
    fxBypassSlot3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (vts, "fxBypass3", fxBypassSlot3);

    addAndMakeVisible(fxBypassSlot1);
    addAndMakeVisible(fxBypassSlot2);
    addAndMakeVisible(fxBypassSlot3);
    
    // Midi keyboard
    addAndMakeVisible(midiKeyboardComponent);

    midiKeyboardComponent.setMidiChannel(2);
    midiKeyboardState.addListener(&audioProcessor.getMidiMessageCollector());

    p.initialiseGraph();
    setSize(900, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    midiKeyboardState.removeListener(&audioProcessor.getMidiMessageCollector());
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    juce::FlexBox fb;
    fb.flexDirection  = juce::FlexBox::Direction::column;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent   = juce::FlexBox::AlignContent::center;

    auto fbWidth = (float) getWidth() / 4.0f;
    auto fbHeight  = (float) getHeight() / 14.0f;
    
    juce::FlexItem FIMuteInput (fbWidth, fbHeight, muteInputButton);

    juce::FlexItem FIOsc1Enabled (fbWidth, fbHeight, osc1Enabled);
    juce::FlexItem FIOsc2Enabled (fbWidth, fbHeight, osc2Enabled);

    juce::FlexItem FIOsc1Waveform (fbWidth, fbHeight, osc1WaveformCB);
    juce::FlexItem FIOsc2Waveform (fbWidth, fbHeight, osc2WaveformCB);

    juce::FlexItem FIFxSlot1 (fbWidth, fbHeight, fxSlot1CB);
    juce::FlexItem FIFxSlot2 (fbWidth, fbHeight, fxSlot2CB);
    juce::FlexItem FIFxSlot3 (fbWidth, fbHeight, fxSlot3CB);
    
    juce::FlexItem FIFxBypassSlot1 (fbWidth, fbHeight, fxBypassSlot1);
    juce::FlexItem FIFxBypassSlot2 (fbWidth, fbHeight, fxBypassSlot2);
    juce::FlexItem FIFxBypassSlot3 (fbWidth, fbHeight, fxBypassSlot3);
    
    fb.items.addArray ({ FIMuteInput,
                         FIOsc1Enabled,
                         FIOsc2Enabled,
                         FIOsc1Waveform,
                         FIOsc2Waveform,
                         FIFxSlot1,
                         FIFxSlot2,
                         FIFxSlot3,
                         FIFxBypassSlot1,
                         FIFxBypassSlot2,
                         FIFxBypassSlot3
                         });

    fb.performLayout (getLocalBounds().toFloat());
    
    auto area = getLocalBounds();
    midiKeyboardComponent.setBounds (area.removeFromTop (80).reduced (8));
    gainSlider.setBounds(20, 20, 360, 30);
}

void AudioPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider){

}
