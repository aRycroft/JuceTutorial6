/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& p, AudioProcessorValueTreeState& state)
    : AudioProcessorEditor (&p), processor (p), params(state)
{
    addSlider("thresh", "Threshold", threshSlider, threshLabel, threshAttachment);
    addSlider("ratio", "Ratio", slopeSlider, slopeLabel, slopeAttachment);
    addSlider("knee", "Knee", kneeSlider, kneeLabel, kneeAttachment);
    addSlider("attack", "Attack", attackSlider, attackLabel, attackAttachment);
    addSlider("release", "Release", releaseSlider, releaseLabel, releaseAttachment);
    setSize (400, 300);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
}

//==============================================================================
void CompressorAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void CompressorAudioProcessorEditor::resized()
{
        threshSlider.setBounds(100, 0, 200, 50);
        slopeSlider.setBounds(100, 50, 200, 50);
        kneeSlider.setBounds(100, 100, 200, 50);
        attackSlider.setBounds(100, 150, 200, 50);
        releaseSlider.setBounds(100, 200, 200, 50);
}

void CompressorAudioProcessorEditor::addSlider(String name, String labelText, Slider& slider, Label& label, std::unique_ptr<SliderAttachment>& attachment) {
    addAndMakeVisible(slider);
    attachment.reset(new SliderAttachment(params, name, slider));

    label.setText(labelText, dontSendNotification);
    label.attachToComponent(&slider, true);
    addAndMakeVisible(label);
}
