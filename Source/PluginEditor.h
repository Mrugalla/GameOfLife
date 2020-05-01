#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class GameOfLifeAudioProcessorEditor  :
    public AudioProcessorEditor,
    public Timer
{
public:
    GameOfLifeAudioProcessorEditor (GameOfLifeAudioProcessor&);
    ~GameOfLifeAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    void timerCallback() override;
    void mouseDown(const MouseEvent&) override;
    void mouseDoubleClick(const MouseEvent&) override;

private:
    HDL::MetaGrid* metagrid;
    int fps{ 6 };
    

    GameOfLifeAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameOfLifeAudioProcessorEditor)
};
