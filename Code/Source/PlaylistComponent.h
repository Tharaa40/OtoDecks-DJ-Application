/*
  ==============================================================================

    PlaylistComponent.h
    Created: 3 Feb 2023 10:33:33am
    Author:  SaungTharaPhi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DeckGUI.h" 
#include <vector>
#include <string>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public TextEditor::Listener
                           
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1,
                      DeckGUI* _deckGUI2);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;


    int getNumRows() override;

    void paintRowBackground(Graphics&,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(Graphics&,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component *existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

     
    void textEditorTextChanged(juce::TextEditor& editor) override;
  

   

private:

    double duration;
    double selectedRow;
    TableListBox tableComponent;
    void deleteTrack(); 
    void loadInDeck(DeckGUI* deckGUI);

    

    TextButton libraryLoad{ "Load into library" };
    TextButton deck1{ "Deck1" };
    TextButton deck2{ "Deck2" };
    TextEditor searchPlaylist{ "Search for tracks" };
    String input;

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    AudioFormatManager formatManager;

    Array<File> myFiles;
    
    Array<String> fileNames;
    String fileName;
    std::vector<std::string> trackTitles; 
    int d_index = -1;
    
    Array <double> trackDurations;
   

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
