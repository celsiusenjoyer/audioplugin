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
    // gainAttachment.reset(new SliderAttachment(valueTreeState, "gain", gainSlider));

    invertButton.setButtonText("Invert Phase");
    addAndMakeVisible(invertButton);
    // invertAttachment.reset(new ButtonAttachment(valueTreeState, "invertPhase", invertButton));

    setSize(paramSliderWidth + paramLabelWidth, juce::jmax(100, paramControlHeight * 2));

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

    gainSlider.setBounds(40, 30, 20, getHeight() - 60);
}
