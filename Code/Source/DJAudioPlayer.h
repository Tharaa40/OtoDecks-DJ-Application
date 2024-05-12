/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 31 Jan 2023 11:27:18am
    Author:  SaungTharaPhi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class DJAudioPlayer : public juce::AudioSource
{
    public:
        DJAudioPlayer(AudioFormatManager& _formatManager);
        ~DJAudioPlayer();
        //==============================================================================
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        void loadURL(juce::URL audioURL);
        void setGain(double gain);
        void setSpeed(double ratio);
        void setPosition(double posInSecs);
        void setPositionRelative(double pos);


        void start();
        void stop();

        /**get the relative position of the playhead*/
        double getPositionRelative();

    private:
       AudioFormatManager& formatManager;
       std::unique_ptr<AudioFormatReaderSource> readerSource;
       AudioTransportSource transportSource;
       ResamplingAudioSource resampleSource{&transportSource, false, 2};



};