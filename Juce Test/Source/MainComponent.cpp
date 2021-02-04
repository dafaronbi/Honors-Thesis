#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    setFramesPerSecond (60);
    addAndMakeVisible(scene);
    getLookAndFeel().setColour (juce::Slider::thumbColourId, juce::Colours::red);
}

MainComponent::~MainComponent()
{
}

void MainComponent::update()
{
    // This function is called at the frequency specified by the setFramesPerSecond() call
    // in the constructor. You can use it to update counters, animate values, etc.
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font ("Times New Roman", 20.0f, juce::Font::bold | juce::Font::italic));
    g.setColour (juce::Colours::peachpuff);
    g.drawText ("Daniel Synth " + currentSizeAsString, getLocalBounds(), juce::Justification::centred, true);
    
    g.setColour (juce::Colours::green);
    g.drawLine (10, 300, 590, 300, 5);
    
    g.setColour (getLookAndFeel().findColour (juce::Slider::thumbColourId));
 
    auto numberOfDots = 15;     // [1]
 
    juce::Path spinePath;       // [2]

    for (auto i = 0; i < numberOfDots; ++i) // [3]
    {
        int radius = 150;

        juce::Point<float> p ((float) getWidth()  / 2.0f + 1.0f * (float) radius * std::sin ((float) getFrameCounter() * 0.04f + (float) i * 0.12f),
                              (float) getHeight() / 2.0f + 1.0f * (float) radius * std::cos ((float) getFrameCounter() * 0.04f + (float) i * 0.12f));
        // draw the circles along the fish
        g.fillEllipse (p.x - i, p.y - i, 2.0f + 2.0f * i, 2.0f + 2.0f * i);
        
        if (i == 0)
            spinePath.startNewSubPath (p);  // if this is the first point, start a new path..
        else
            spinePath.lineTo (p);           // ...otherwise add the next point
    }
 
        // draw an outline around the path that we have created
        g.strokePath (spinePath, juce::PathStrokeType (4.0f)); // [4]
}
    

void MainComponent::resized()
{
    currentSizeAsString = juce::String (getWidth()) + " x " + juce::String (getHeight());
    scene.setBounds (getWidth()/4, getHeight()/4, getWidth()/2, getHeight()/2);
}
