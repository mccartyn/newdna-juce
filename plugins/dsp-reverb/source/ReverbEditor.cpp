
#include "ReverbProcessor.h"
#include "ReverbEditor.h"

ReverbAudioProcessorEditor::ReverbAudioProcessorEditor (ReverbAudioProcessor& p, 
                                                                    juce::UndoManager& um)
    : AudioProcessorEditor (&p), editorContent (p, um)
{
    const auto ratio = static_cast<double> (defaultWidth) / defaultHeight;
    setResizable (false, true);
    getConstrainer()->setFixedAspectRatio (ratio);
    getConstrainer()->setSizeLimits (defaultWidth, defaultHeight, defaultWidth * 2, defaultHeight * 2);
    setSize (defaultWidth, defaultHeight);
    editorContent.setSize (defaultWidth, defaultHeight);

    addAndMakeVisible (editorContent);
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
}

void ReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::black);
}

void ReverbAudioProcessorEditor::resized()
{
    const auto factor = static_cast<float> (getWidth()) / defaultWidth;
    editorContent.setTransform (juce::AffineTransform::scale (factor));
}
