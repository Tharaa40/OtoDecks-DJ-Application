/*
  ==============================================================================

    DeckGUI.cpp
    Created: 31 Jan 2023 12:49:39pm
    Author:  SaungTharaPhi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"


//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager & formatManagerToUse,
                 AudioThumbnailCache & cacheToUse) 
    : player(_player), 
    waveformDisplay(formatManagerToUse, cacheToUse)


{
    /*To add component and make them visible on the deck*/
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(clearButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);

    /*Adding event listeners to the buttons and sliders*/
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);
    clearButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    /*Setting a range to the sliders*/
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);

    /*Setting a text to the labels and assigning them to a specific slider*/
    volume.setText("Volume", dontSendNotification);
    speed.setText("Speed", dontSendNotification);
    position.setText("Position", dontSendNotification);

    volume.attachToComponent(&volSlider, false);
    volume.setJustificationType(Justification::centred);

    speed.attachToComponent(&speedSlider, false);
    speed.setJustificationType(Justification::centred);

    position.attachToComponent(&posSlider, false);
    position.setJustificationType(Justification::centred);

    startTimer(500);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g )
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.fillAll(Colours::cadetblue);
 
    /*Color of the text*/
    g.setColour (juce::Colours::black);
    g.setFont (14.0f);

    /*Customizing the color and style of the text buttons, sliders and labels*/
    getLookAndFeel().setColour(TextButton::ColourIds::buttonColourId, Colours::lightcyan); //changed all the colours of the text buttons
    getLookAndFeel().setColour(TextButton::textColourOffId, Colours::black);
  
    getLookAndFeel().setColour(Slider::trackColourId, Colours::aliceblue);
    getLookAndFeel().setColour(Slider::textBoxTextColourId, Colours::black);
    getLookAndFeel().setColour(Label::textColourId, Colours::black);
    
    volSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
   

    speedSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);

    posSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    posSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
}


void DeckGUI::resized()
{

    double rowH = getHeight() / 7;
    double rowW = getWidth() / 3;
    int margin = 5;
    playButton.setBounds(margin, margin, rowW-margin, rowH);
    stopButton.setBounds(rowW+margin, margin, rowW-margin, rowH);
    loadButton.setBounds((rowW*2)+margin , margin, rowW-margin, rowH);
    loopButton.setBounds(margin, rowH + margin * 2, rowW, rowH);
    clearButton.setBounds(rowW + margin, rowH + margin * 2, rowW, rowH);
    waveformDisplay.setBounds(0, rowH*2, getWidth(), rowH * 2);

    volSlider.setBounds(margin*2, (rowH * 4)+margin, rowW-(margin*7), rowH+(margin*12));
    speedSlider.setBounds(rowW+margin*2, (rowH * 4) + margin, rowW - (margin * 7), rowH + (margin * 12));
    posSlider.setBounds(rowW*2+margin*2, (rowH * 4) + margin, rowW - (margin * 7), rowH + (margin * 12));

    volume.setBounds(margin*2, (rowH*5)+20 , rowW - (margin*7), rowH + (margin * 12));
    speed.setBounds(rowW+margin*2, (rowH*5)+20, rowW - (margin*7), rowH + (margin * 12));
    position.setBounds(rowW*2+margin*2, (rowH*5)+20, rowW - (margin*7), rowH + (margin * 12));
}




void DeckGUI::buttonClicked(Button* button) /*Functionality of different buttons on the deck*/
{
    if(button == &playButton)
    {
        DBG("Play button was clicked");
        player->start();

    }
    if (button == &stopButton)
    {
        DBG("Stop button was clicked");
        player->stop();
    }
    if(button == &loadButton)
    {
        FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(juce::URL{chooser.getResult()});
            waveformDisplay.loadURL(juce::URL{ chooser.getResult()});
        }
    }
    if(button == &clearButton)
    {
        if(clearButton.getToggleState())
        {
            waveformDisplay.clear();
        }
    }
}

void DeckGUI::sliderValueChanged(Slider* slider) /*Changing the sliders*/
{
    if(slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if(slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if(slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray &files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const StringArray &files, int x, int y) /*Dropping the file into the deck*/
{
    for(String filename:files)
    {
        if (files.size() == 1)
        {
            DBG("DeckGUI::filesDropped");
            URL fileURL = URL{ File{filename} };
            player->loadURL(fileURL);
            waveformDisplay.loadURL(URL{ File{files[0]} });
        }
    }
}


void DeckGUI::loadedDeck(URL audioURL) 
{
    DBG("Loaded deck called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}


void DeckGUI::timerCallback()
{
    if (std::to_string(loopButton.getToggleState()) == "0")
    {
        if (player->getPositionRelative() >= 1)
        {
            player->setPositionRelative(0);
            player->stop();
        }
    }

    if (std::to_string(loopButton.getToggleState()) == "1") 
    { 
        if (player->getPositionRelative() >= 1)
        {
            player->setPositionRelative(0); 
            player->start();
        }
    }
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}



