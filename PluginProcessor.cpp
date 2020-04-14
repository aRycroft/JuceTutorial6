/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessor::CompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
    ,
     state(*this, nullptr, Identifier("params"), {
          std::make_unique<AudioParameterFloat>(
              "thresh", 
              "Threshold", 
              NormalisableRange<float>(-60.0f, 20.0f, 0.01f), 
              10.0f),
          std::make_unique<AudioParameterFloat>(
              "ratio",
              "Ratio", 
              NormalisableRange<float>(1.0f, 20.0f, 0.01f), 
              2.0f),
          std::make_unique<AudioParameterFloat>(
              "knee", 
              "KneeWidth", 
              NormalisableRange<float>(0.0f, 24.0f, 0.01f), 
              0.0f),
          std::make_unique<AudioParameterFloat>(
              "attack", 
              "Attack", 
              NormalisableRange<float>(0.01f, 500.0, 0.01f), 
              100.0f),
          std::make_unique<AudioParameterFloat>(
              "release", 
              "Release", 
              NormalisableRange<float>(0.01f, 2000.0f, 0.01f), 
              500.0f)
        }
       )
#endif
{
}

CompressorAudioProcessor::~CompressorAudioProcessor()
{
}

//==============================================================================
const String CompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorAudioProcessor::setCurrentProgram (int index)
{
}

const String CompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompressorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int channel = 0; channel < getNumOutputChannels(); channel++) {
        allCompressors.add(Compressor());
    }
    threshParam = state.getRawParameterValue("thresh");
    slopeParam = state.getRawParameterValue("ratio");
    kneeParam = state.getRawParameterValue("knee");
    attackParam = state.getRawParameterValue("attack");
    releaseParam = state.getRawParameterValue("release");
}

void CompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void CompressorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    float at = 1 - std::pow(MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (*attackParam / 1000.0f));
    float rt = 1 - std::pow(MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (*releaseParam / 1000.0f));

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        for (int channel = 0; channel < getTotalNumOutputChannels(); channel++) {
            auto* data = buffer.getWritePointer(channel);
            Compressor* comp = &allCompressors.getReference(channel);
            data[i] = comp->compressSample(data[i], *threshParam, *slopeParam, at, rt, *kneeParam);
        }
    }
}

//==============================================================================
bool CompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
    return new CompressorAudioProcessorEditor (*this, state);
}

//==============================================================================
void CompressorAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    auto stateTree = state.copyState();
    std::unique_ptr<XmlElement> xml(stateTree.createXml());
    copyXmlToBinary(*xml, destData);
}

void CompressorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr && xmlState->hasTagName(state.state.getType())) {
        state.replaceState(ValueTree::fromXml(*xmlState));
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAudioProcessor();
}
