#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p,
                                                                  juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    gainLabel.setText("Gain", juce::dontSendNotification);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(gainSlider);

    invertButton.setButtonText("Invert Phase");
    addAndMakeVisible(invertButton);
   
    // Mute input
    muteInputButton.setButtonText("Mute Input");
    muteInputButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(muteInputButton);

    // Combo box
    fxSlot1Label.setText("FX Slot 1", juce::dontSendNotification);
    fxSlot2Label.setText("FX Slot 2", juce::dontSendNotification);
    fxSlot3Label.setText("FX Slot 3", juce::dontSendNotification);

    addAndMakeVisible(fxSlot1Label);
    addAndMakeVisible(fxSlot2Label);
    addAndMakeVisible(fxSlot3Label);

    fxSlot1CB.addItemList(processorChoices, 1);
    fxSlot2CB.addItemList(processorChoices, 1);
    fxSlot3CB.addItemList(processorChoices, 1);

    addAndMakeVisible(fxSlot1CB);
    addAndMakeVisible(fxSlot2CB);
    addAndMakeVisible(fxSlot3CB);
    
    // Bypass buttons
    fxBypassSlot1.setButtonText("FX Bypass 1");
    fxBypassSlot2.setButtonText("FX Bypass 2");
    fxBypassSlot3.setButtonText("FX Bypass 3");

    addAndMakeVisible(fxBypassSlot1);
    addAndMakeVisible(fxBypassSlot2);
    addAndMakeVisible(fxBypassSlot3);


    p.initialiseGraph();
    setSize(600, 400);

    /* MIDI SLIDER
    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Volume");
    midiVolume.setValue(1.0);
    midiVolume.addListener(this);

    addAndMakeVisible(&midiVolume);
    */

}


void AudioPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.noteOnVel = midiVolume.getValue();
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    invertAttachment.reset(new ButtonAttachment(valueTreeState, "invertPhase", invertButton));

    gainAttachment.reset(new SliderAttachment(valueTreeState, "gain", gainSlider));

    muteInputAttachment.reset(new ButtonAttachment(valueTreeState, "muteInput", muteInputButton));

    fxSlot1CBAttachment.reset(new ComboBoxAttachment(valueTreeState, "fxSlot1", fxSlot1CB));
    fxSlot2CBAttachment.reset(new ComboBoxAttachment(valueTreeState, "fxSlot2", fxSlot2CB));
    fxSlot3CBAttachment.reset(new ComboBoxAttachment(valueTreeState, "fxSlot3", fxSlot3CB));

    fxBypassSlot1Attachment.reset(new ButtonAttachment(valueTreeState, "fxBypass1", fxBypassSlot1));
    fxBypassSlot2Attachment.reset(new ButtonAttachment(valueTreeState, "fxBypass2", fxBypassSlot2));
    fxBypassSlot3Attachment.reset(new ButtonAttachment(valueTreeState, "fxBypass3", fxBypassSlot3));
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

    // midiVolume.setBounds(40, 30, 20, getHeight() - 60);
    juce::FlexBox fb;
    fb.flexDirection  = juce::FlexBox::Direction::column;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent   = juce::FlexBox::AlignContent::center;

    auto fbWidth = (float) getWidth() / 2.0f;
    auto fbHeight  = (float) getHeight() / 7.0f;
    
    juce::FlexItem FIMuteInput (fbWidth, fbHeight, muteInputButton);

    juce::FlexItem FIFxSlot1 (fbWidth, fbHeight, fxSlot1CB);
    juce::FlexItem FIFxSlot2 (fbWidth, fbHeight, fxSlot2CB);
    juce::FlexItem FIFxSlot3 (fbWidth, fbHeight, fxSlot3CB);
    
    juce::FlexItem FIFxBypassSlot1 (fbWidth, fbHeight, fxBypassSlot1);
    juce::FlexItem FIFxBypassSlot2 (fbWidth, fbHeight, fxBypassSlot2);
    juce::FlexItem FIFxBypassSlot3 (fbWidth, fbHeight, fxBypassSlot3);
    
    fb.items.addArray ({ FIMuteInput,
                         FIFxSlot1,
                         FIFxSlot2,
                         FIFxSlot3,
                         FIFxBypassSlot1,
                         FIFxBypassSlot2,
                         FIFxBypassSlot3
                         });

    fb.performLayout (getLocalBounds().toFloat());

    gainSlider.setBounds(40, 30, 20, getHeight() - 60);
}
