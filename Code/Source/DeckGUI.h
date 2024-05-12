/*
  ==============================================================================

    DeckGUI.h
    Created: 31 Jan 2023 12:49:39pm
    Author:  SaungTharaPhi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public Component,
                 public Button::Listener,
                 public Slider::Listener,
                 public FileDragAndDropTarget,
                 public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager& formatManagerToUse,
            AudioThumbnailCache& cacheToUse); 
    
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;
 
    void buttonClicked(Button*) override; /*Implement Button Listener*/
    void sliderValueChanged(Slider* slider) override; /*Implement Slider::Listener*/
    bool isInterestedInFileDrag(const StringArray &files) override; /**Detects if file is being dragged over deck*/
    void filesDropped(const StringArray &files, int x, int y) override; /*To drop the file onto deck*/
    void timerCallback() override; /*Listen for changes to the waveform*/
    void loadedDeck(URL audioURL); /*To implement player and waveform URL */


private:

    /*Creating text and toggle buttons*/
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };
    ToggleButton loopButton{ "LOOP" };
    ToggleButton clearButton{ "CLEAR" };

    /*Creating sliders*/
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    /*Creating labels for sliders*/
    Label volume;
    Label speed;
    Label position;

    DJAudioPlayer* player;

    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
