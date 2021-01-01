/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainPlugin3AudioProcessor::GainPlugin3AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), //gainValue(-5.0f),
    treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    //treeState.state = ValueTree("savedParams");
}

GainPlugin3AudioProcessor::~GainPlugin3AudioProcessor()
{
}


//==============================================================================
//Parameter Layout
AudioProcessorValueTreeState::ParameterLayout GainPlugin3AudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr <RangedAudioParameter >> params;

    auto gainParam = std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, -48.0f, 0.0f, -15.0f);

    params.push_back(std::move(gainParam));

    return { params.begin(), params.end() };

}

const juce::String GainPlugin3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainPlugin3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GainPlugin3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GainPlugin3AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GainPlugin3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainPlugin3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GainPlugin3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void GainPlugin3AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GainPlugin3AudioProcessor::getProgramName (int index)
{
    return {};
}

void GainPlugin3AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GainPlugin3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    previousGain = pow(10, *treeState.getRawParameterValue(GAIN_ID)/20);
    //previousGain = juce::Decibels::decibelsToGain(treeState.getRawParameterValue(GAIN_ID));
}

void GainPlugin3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainPlugin3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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
#endif

void GainPlugin3AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float currentGain = pow(10, *treeState.getRawParameterValue(GAIN_ID)/20);

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    if (currentGain == previousGain) {
        buffer.applyGain(currentGain);
    }
    else {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }
        
}

//==============================================================================
bool GainPlugin3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GainPlugin3AudioProcessor::createEditor()
{
    return new GainPlugin3AudioProcessorEditor (*this);
}

//==============================================================================
void GainPlugin3AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    //ScopedPointer <XmlElement> xml (treeState.state.createXml());

    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);

}

void GainPlugin3AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(treeState.state.getType()))
        {
            treeState.state = juce::ValueTree::fromXml(*xmlState);
        }
    }
}
    

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainPlugin3AudioProcessor();
}
