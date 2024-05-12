/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 31 Jan 2023 6:21:00pm
    Author:  SaungTharaPhi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse) : 
                                 audioThumb(1000, formatManagerToUse, cacheToUse),
                                 fileLoaded(false), 
                                    position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);

}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.fillAll(Colours::cadetblue);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component


    if(fileLoaded)
    {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        g.setColour(juce::Colours::lightgreen);
        g.drawEllipse(position * getWidth(), 0, getWidth() / 20, getHeight(), 1.0f); //shows where the current track is playing at 
    }
    else
    {
       g.setFont(20.0f);
       g.drawText("File not loaded...", getLocalBounds(),
                   juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}


void WaveformDisplay::loadURL(URL audioURL) /*Loading URL onto the deck to display the waveform*/
{
    DBG("wfd: loadURL");
    audioThumb.clear();
    fileLoaded=  audioThumb.setSource(new URLInputSource(audioURL));
    if(fileLoaded)
    {
        DBG("wfd: loaded");
        repaint();
        
    }
    else
    {
        DBG("wfd: not loaded");
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    DBG("wfd: change received");
    repaint();
}



void WaveformDisplay::setPositionRelative(double pos) //this is to move the position of the audioThumb
{
    if(pos != position && pos > 0)
    {
        position = pos;
        repaint();
        DBG("position moving");
    }

}

void WaveformDisplay::clear() /*remove the waveform after clicking the on 'clear' toggle button*/
{
    audioThumb.clear();
    fileLoaded = false;
    repaint();
}