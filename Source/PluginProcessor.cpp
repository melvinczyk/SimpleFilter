/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleFilterAudioProcessor::SimpleFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
    :
#endif
   parameters(*this, nullptr, juce::Identifier("SimpleFilterPlugin"),
              {std::make_unique<juce::AudioParameterFloat>("cutoff_frequency", "Cutoff Frequency", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.2f, false}, 500.f),
       std::make_unique<juce::AudioParameterBool>("highpass", "Highpass", false)}){
           cutoffFrequencyParameter = parameters.getRawParameterValue("cutoff_frequency");
           highpassParameter = parameters.getRawParameterValue("highpass");
}

SimpleFilterAudioProcessor::~SimpleFilterAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    filter.setSampleRate(static_cast<float>(sampleRate));
}

void SimpleFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void SimpleFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    const auto cutuffFreq = cutoffFrequencyParameter->load();
    const auto highpass = *highpassParameter < 0.5f ? false : true;
    
    filter.setCuttoff(cutuffFreq);
    filter.setHighPass(highpass);
    
    filter.processBlock(buffer, midiMessages);
}

//==============================================================================
bool SimpleFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleFilterAudioProcessor::createEditor()
{
    return new SimpleFilterAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void SimpleFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleFilterAudioProcessor();
}
