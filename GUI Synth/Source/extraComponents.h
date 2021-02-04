/*
  ==============================================================================

    Components.h
    Created: 4 Feb 2021 1:34:43pm
    Author:  Daniel Faronbi
    Use:    GUI components for synthesizer

  ==============================================================================
*/

#pragma once

//Gui for nav bar
class Nav_Bar : public juce::AnimatedAppComponent
{
public:
    //==============================================================================
    Nav_Bar()
    {
    }

    void update() override
    {
        // This function is called at the frequency specified by the setFramesPerSecond() call
        // in the constructor. You can use it to update counters, animate values, etc.
    }

    void paint (juce::Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (juce::Colours::silver);
        g.setColour (juce::Colours::peachpuff);
        g.drawText("Nav Bar",getLocalBounds(), juce::Justification::centred, true);
    }

    void resized() override
    {
        
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }

private:
    juce::TextButton
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Nav_Bar)
};

//gui for filter menu
class Filter_Menu : public juce::AnimatedAppComponent
{
public:
    //==============================================================================
    Filter_Menu()
    {
    }

    void update() override
    {
        // This function is called at the frequency specified by the setFramesPerSecond() call
        // in the constructor. You can use it to update counters, animate values, etc.
    }

    void paint (juce::Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (juce::Colours::lavenderblush);
        g.setColour (juce::Colours::peachpuff);
        g.drawText("Filter MENU",getLocalBounds(), juce::Justification::centred, true);
    }

    void resized() override
    {
        
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter_Menu)
};
