/*
  ==============================================================================

   Copyright 2021, 2022 Suzuki Kengo

   Simple Reverb is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
 
   Simple Reverb is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with Simple Reverb. If not, see <http://www.gnu.org/licenses/>.

  ==============================================================================
*/

#include "EditorContent.h"
#include "ParamIDs.h"
#include "MyColours.h"

EditorContent::EditorContent (ReverbAudioProcessor& p, juce::UndoManager& um)
    : apvts (p.getPluginState()), undoManager (um),
      sizeDial  (*apvts.getParameter (ParamIDs::size),  &um),
      dampDial  (*apvts.getParameter (ParamIDs::damp),  &um),
      widthDial (*apvts.getParameter (ParamIDs::width), &um),
      mixDial   (*apvts.getParameter (ParamIDs::mix),   &um),
      freezeAttachment (*apvts.getParameter (ParamIDs::freeze), freezeButton, &um)
{
    setWantsKeyboardFocus (true);

    addAndMakeVisible (sizeDial);
    addAndMakeVisible (dampDial);
    addAndMakeVisible (widthDial);
    addAndMakeVisible (mixDial);
    addAndMakeVisible (freezeButton);
}

void EditorContent::resized()
{
    const juce::Rectangle<int> baseDialBounds { 0, 73, 80, 96 };
    sizeDial.setBounds  (baseDialBounds.withX (46));
    dampDial.setBounds  (baseDialBounds.withX (144));
    widthDial.setBounds (baseDialBounds.withX (342));
    mixDial.setBounds   (baseDialBounds.withX (440));

    freezeButton.setBounds (249, 110, 68, 32);
}

bool EditorContent::keyPressed (const juce::KeyPress& k)
{
    const auto cmdZ = juce::KeyPress ('z', juce::ModifierKeys::commandModifier, 0);

    if (k == cmdZ && undoManager.canUndo())
    {
        undoManager.undo();
        return true;
    }

    const auto cmdShiftZ = juce::KeyPress ('z', juce::ModifierKeys::commandModifier 
                                                | juce::ModifierKeys::shiftModifier, 0);

    if (k == cmdShiftZ && undoManager.canRedo())
    {
        undoManager.redo();
        return true;
    }

    return false;
}
