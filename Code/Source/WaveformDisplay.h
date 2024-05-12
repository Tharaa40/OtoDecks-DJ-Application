/*
  ==============================================================================

    WaveformDisplay.h
    Created: 31 Jan 2023 6:21:00pm
    Author:  SaungTharaPhi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager & formatManagerToUse,
                    AudioThumbnailCache & cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void loadURL(URL audioURL);


    /**set the relative position of the playhead*/
    void setPositionRelative(double pos);
    void clear();


private:
    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
