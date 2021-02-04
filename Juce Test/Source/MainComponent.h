#pragma once

#include <JuceHeader.h>


class SceneComponent    : public juce::Component
{
public:
    SceneComponent()
    {
        addAndMakeVisible(dial1);
        addAndMakeVisible(dial2);
        button1.setButtonText("Hi");
        button2.setButtonText("There");
        addAndMakeVisible(button1);
        addAndMakeVisible(button2);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::lightblue);
    }

    void resized() override
    {
        dial1.setBounds (0, 0, 200, 100);
        dial2.setBounds (getWidth()/2, 0, 200, 100);
        button1.setBounds (0, getWidth()/2, 60, 30);
        button2.setBounds (getWidth()/2, getWidth()/2, 60, 30);
    }

private:
    juce::Slider dial1;
    juce::Slider dial2;
    juce::TextButton button1;
    juce::TextButton button2;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneComponent)
};
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AnimatedAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void update() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    juce::String currentSizeAsString;
    SceneComponent scene;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


