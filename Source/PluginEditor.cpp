/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainPlugin3AudioProcessorEditor::GainPlugin3AudioProcessorEditor (GainPlugin3AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 400);

    volumeSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    volumeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 120, 20);
    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);

    addAndMakeVisible(volumeSlider);
}

GainPlugin3AudioProcessorEditor::~GainPlugin3AudioProcessorEditor()
{
}

//==============================================================================
void GainPlugin3AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void GainPlugin3AudioProcessorEditor::resized()
{
    volumeSlider.setBounds(getLocalBounds());
}

void GainPlugin3AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if(slider == &volumeSlider)
    {
        audioProcessor.rawVolume = volumeSlider.getValue();
    }
}
