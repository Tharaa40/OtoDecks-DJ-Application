/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 3 Feb 2023 10:33:33am
    Author:  SaungTharaPhi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <string>



//==============================================================================

PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                    DeckGUI* _deckGUI2):deckGUI1(_deckGUI1),
                                                         deckGUI2(_deckGUI2)
                                
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    

    tableComponent.getHeader().addColumn("Track title", 0, 264);
    tableComponent.getHeader().addColumn("Length", 1, 263);
    tableComponent.getHeader().addColumn("Remove", 2, 263);
    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(libraryLoad);
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    addAndMakeVisible(searchPlaylist);

    libraryLoad.addListener(this);
    deck1.addListener(this);
    deck2.addListener(this);
    searchPlaylist.addListener(this);
    formatManager.registerBasicFormats();

    duration = 0.0;
    selectedRow = 0.0;
}



PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::black);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text

    getLookAndFeel().setColour(TextEditor::ColourIds::backgroundColourId, Colours::lightcyan);
    searchPlaylist.setJustification(Justification::centred);
    getLookAndFeel().setColour(TextEditor::textColourId, Colours::black);
    searchPlaylist.setFont(14.0f);
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    double rowH = getHeight() / 7;
    double rowW = getWidth()/2;
    int buttonWidth = 130;
    int buttonHeight = rowH * 1.1;
    int buttonSpacing = 20;
    tableComponent.setBounds(0, 0, rowW + 150, rowH * 5);
    libraryLoad.setBounds(rowW+150 + buttonSpacing, 10, buttonWidth, buttonHeight);
    deck1.setBounds(libraryLoad.getRight() + buttonSpacing, 10, buttonWidth, buttonHeight);
    deck2.setBounds(deck1.getRight() + buttonSpacing, 10, buttonWidth, buttonHeight);
    searchPlaylist.setBounds(rowW+150+ buttonSpacing, rowH*1.5, buttonWidth*3.3, rowH * 1.5);
}


int PlaylistComponent::getNumRows() /*Displaying the files that are in the trackTitles vector */
{
    return trackTitles.size();
    DBG(trackTitles.size());
}



void PlaylistComponent::paintRowBackground(Graphics & g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected) 
{
    if(rowIsSelected)
    {
        g.fillAll(Colours::beige);
    }
    else {
        g.fillAll(Colours::darkgrey);
    }
}


void PlaylistComponent::paintCell(Graphics & g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected) 
{
    if(columnId == 0) /*Displaying fileNames in the relevant column*/
    {
        g.drawText(fileNames[rowNumber],
            2, 0,
            width - 4, height,
            Justification::centredLeft,
            true);
    }
    
    if(columnId == 1) /*Displaying duration in the relevant column*/
    {
        int final_Duration = trackDurations[rowNumber];
        int hours = final_Duration / 3600;
        int min = (final_Duration % 3600) / 60;
        int sec = (final_Duration % 60);
        String durationStr = String(hours) + ":" + String(min) + ":" + String(sec);
        g.drawText(durationStr, 2, 0,
            width - 4, height,
            Justification::centredLeft,
            true);
    }
    
}





Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component *existingComponentToUpdate)
{
    if(columnId == 2) /*Creating the delete button*/
    {
        if(existingComponentToUpdate == nullptr)
        {
            TextButton* btn3 = new TextButton("Delete");
            String id{std::to_string(rowNumber)};
            btn3->setComponentID(id);
            btn3->addListener(this);
            existingComponentToUpdate = btn3;
            btn3->onClick = [this] {deleteTrack(); }; /*Functionality of the delete button */
        }
    }
    return existingComponentToUpdate;
    
}


void  PlaylistComponent::textEditorTextChanged(juce::TextEditor& editor)  /*Text editor to search for the relevant track based on the input*/
{
    
    input = searchPlaylist.getText();
    
    for(int i=0; i<fileNames.size(); ++i)
    {
        if(fileNames[i].containsWholeWordIgnoreCase(input) && input != " ")
        {
            tableComponent.selectRow(i, false, true);
        }
    }
}

void PlaylistComponent::buttonClicked(Button* button) /*Functionality of the buttons in playlistcomponent*/
{
    if(button == &libraryLoad)
    {
        FileChooser choosers{ "Select a file to add to the library" };
        if(choosers.browseForMultipleFilesToOpen()) //allows user to select multiple files
        {
            myFiles = choosers.getResults(); //store the results in an array of files
            for(int i=0; i<myFiles.size(); ++i) //looping over the array of files
            {
                if(trackTitles.size() != 0 || i==0) //if there are no tracks in the vector or index is equal to 0
                {
                    for(int j=0; j<trackTitles.size(); ++j) //loop through the vector of tracks
                    {
                        if(myFiles[i].getFileName().toStdString() == trackTitles[j]) //if the choice is equal to what is already in the vector 
                        {
                            DBG("duplicate");
                            d_index = i; //take note of the index of the duplicated file
                            break;
                        }
                    }
                    if(i != d_index) //if it is not equal 
                    {
                        fileName = URL{ myFiles[i] }.getFileName(); //creates a URL of the choices, get the name 
                        fileNames.add(fileName); //store the names in fileNames 
                        AudioFormatReader* reader = formatManager.createReaderFor(myFiles[i]);
                        duration = reader->lengthInSamples / reader->sampleRate; //read the length of the file
                        trackDurations.add(duration); //store the length of the files in trackDurations
                        trackTitles.push_back(myFiles[i].getFileName().toStdString());
                    }
                }
            }
            DBG("table update");
            tableComponent.updateContent();
        }
    }
    if(button == &deck1)
    {
        loadInDeck(deckGUI1); //load the track in deck1
    }
    if(button == &deck2)
    {
        loadInDeck(deckGUI2); //load the track in deck2
    }
    
    selectedRow = button->getComponentID().getDoubleValue();  
}

void PlaylistComponent::loadInDeck(DeckGUI* deckGUI)  //to check if a track has been selected and pass it into deckGUI
{
   for(int i=0; i<myFiles.size(); ++i)
   {
       int loadRow=  tableComponent.getSelectedRow() ;
       if (loadRow != -1)
       {
           DBG("deck1load");
           deckGUI->loadedDeck(URL{ File{myFiles[i] }});
           DBG("DECKGUI LOADINDECK");
       }
   }
}
 
void PlaylistComponent::deleteTrack() //to delete the track functionality
{
    DBG(selectedRow);
    for(int i=0; i<trackTitles.size(); ++i)
    {
        if(selectedRow == i)
        {
            fileNames.remove(i);
            trackTitles.erase(trackTitles.begin() + i);
        }
    }
    tableComponent.updateContent();
}


