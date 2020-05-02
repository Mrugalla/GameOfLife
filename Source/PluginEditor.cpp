#include "PluginProcessor.h"
#include "PluginEditor.h"

GameOfLifeAudioProcessorEditor::GameOfLifeAudioProcessorEditor (GameOfLifeAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p){
    metagrid = &processor.metagrid;
    setResizable(true, true);
    setSize (800, 800);
}

GameOfLifeAudioProcessorEditor::~GameOfLifeAudioProcessorEditor(){}

void GameOfLifeAudioProcessorEditor::timerCallback() {
    metagrid->onUpdate();
    repaint();
}

void GameOfLifeAudioProcessorEditor::paint (Graphics& g){
    g.fillAll(Colours::black);
    static auto hue = 0.f;
    metagrid->draw(g, Colour(255,0,0).withRotatedHue(hue));
    hue += .05f;
    if (hue > 1.f)
        hue -= 1.f;
}

void GameOfLifeAudioProcessorEditor::resized(){ metagrid->setGrid(24, getLocalBounds()); }

void GameOfLifeAudioProcessorEditor::mouseDown(const MouseEvent& evt) {
    metagrid->onClick(evt);
    if (!isTimerRunning())
        repaint();
}

void GameOfLifeAudioProcessorEditor::mouseDoubleClick(const MouseEvent& evt) {
    if (isTimerRunning())
        stopTimer();
    else
        startTimerHz(fps);
}