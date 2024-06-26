#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
  
    setSize(800, 700);


    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);

    }
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) // This function will be called when the audio device is started, or when its settings (i.e. sample rate, block size, etc) are changed.                                                                                
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

}



void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)   //audio-processing code goes here
{
    mixerSource.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources() // This will be called when the audio device stops, or when it is being restarted due to a setting change.
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();

}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    int margin = 5;

    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() / 2);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);

    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);

}










