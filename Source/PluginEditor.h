/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;

//==============================================================================
/**
*/
class GainPlugin3AudioProcessorEditor  : public juce::AudioProcessorEditor,public Slider::Listener
{
public:
    GainPlugin3AudioProcessorEditor (GainPlugin3AudioProcessor&);
    ~GainPlugin3AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainPlugin3AudioProcessor& audioProcessor;

    Slider volumeSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainPlugin3AudioProcessorEditor)
};
