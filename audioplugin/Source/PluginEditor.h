#pragma once

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public  juce::AudioProcessorEditor,
                                              private juce::Slider::Listener
{
public:
    enum{
        paramControlHeight = 40,
        paramLabelWidth  = 80,
        paramSliderWidth   = 300
    };

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    void sliderValueChanged(juce::Slider* slider) override;
    
    const juce::StringArray processorChoices { "Empty", "Oscillator", "Gain", "Filter" };
    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::Label gainLabel;
    juce::Slider gainSlider;
    std::unique_ptr<SliderAttachment> gainAttachment;

    juce::ToggleButton invertButton;
    std::unique_ptr<ButtonAttachment> invertAttachment;

    juce::Slider midiVolume;
    
    juce::ToggleButton muteInputButton;
    std::unique_ptr<ButtonAttachment> muteInputAttachment;
    
    juce::Label fxSlot1Label; 
    juce::Label fxSlot2Label;
    juce::Label fxSlot3Label; 

    juce::ComboBox fxSlot1CB;
    juce::ComboBox fxSlot2CB;
    juce::ComboBox fxSlot3CB;

    std::unique_ptr<ComboBoxAttachment> fxSlot1CBAttachment;
    std::unique_ptr<ComboBoxAttachment> fxSlot2CBAttachment;
    std::unique_ptr<ComboBoxAttachment> fxSlot3CBAttachment;

    juce::ToggleButton fxBypassSlot1;
    juce::ToggleButton fxBypassSlot2;
    juce::ToggleButton fxBypassSlot3;

    std::unique_ptr<ButtonAttachment> fxBypassSlot1Attachment;
    std::unique_ptr<ButtonAttachment> fxBypassSlot2Attachment;
    std::unique_ptr<ButtonAttachment> fxBypassSlot3Attachment;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
