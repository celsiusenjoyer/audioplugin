#pragma once

#include "PluginProcessor.h"
#include <juce_audio_utils/juce_audio_utils.h>

//==============================================================================
class AudioPluginAudioProcessorEditor final : public  juce::AudioProcessorEditor,
                                              private juce::Slider::Listener
{
public:

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider*);
    
private:
    juce::AudioProcessorValueTreeState& valueTreeState;

    const juce::StringArray processorChoices { "Empty", "Oscillator", "Gain", "Filter" };
    const juce::StringArray waveformChoices { "sinewave", "sawtooth" };
    
    // Invert Phase
    juce::ToggleButton invertButton;
    std::unique_ptr<ButtonAttachment> invertAttachment;

    // Gain
    juce::Label gainLabel;
    juce::Slider gainSlider;
    std::unique_ptr<SliderAttachment> gainAttachment;

    // Mute
    juce::ToggleButton muteInputButton;
    std::unique_ptr<ButtonAttachment> muteInputAttachment;
    
    // Osc Waveform 
    juce::Label osc1WaveformLabel;
    juce::Label osc2WaveformLabel;
    juce::ComboBox osc1WaveformCB;
    juce::ComboBox osc2WaveformCB;
    std::unique_ptr<ComboBoxAttachment> osc1WaveformCBAttachment;
    std::unique_ptr<ComboBoxAttachment> osc2WaveformCBAttachment;

    // Oscillators enabled
    juce::ToggleButton osc1Enabled;
    juce::ToggleButton osc2Enabled;
    std::unique_ptr<ButtonAttachment> osc1EnabledAttachment;
    std::unique_ptr<ButtonAttachment> osc2EnabledAttachment;

    // FX Slot Combo box
    juce::Label fxSlot1Label; 
    juce::Label fxSlot2Label;
    juce::Label fxSlot3Label; 

    juce::ComboBox fxSlot1CB;
    juce::ComboBox fxSlot2CB;
    juce::ComboBox fxSlot3CB;

    std::unique_ptr<ComboBoxAttachment> fxSlot1CBAttachment;
    std::unique_ptr<ComboBoxAttachment> fxSlot2CBAttachment;
    std::unique_ptr<ComboBoxAttachment> fxSlot3CBAttachment;
    
    // FX Bypass buttons
    juce::ToggleButton fxBypassSlot1;
    juce::ToggleButton fxBypassSlot2;
    juce::ToggleButton fxBypassSlot3;

    std::unique_ptr<ButtonAttachment> fxBypassSlot1Attachment;
    std::unique_ptr<ButtonAttachment> fxBypassSlot2Attachment;
    std::unique_ptr<ButtonAttachment> fxBypassSlot3Attachment;
    
    // Midi keyboard
    juce::MidiKeyboardState midiKeyboardState;
    juce::MidiKeyboardComponent midiKeyboardComponent { midiKeyboardState, juce::MidiKeyboardComponent::horizontalKeyboard };

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
