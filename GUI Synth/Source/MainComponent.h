#pragma once
#include <JuceHeader.h>
#include "extraComponents.h"
#include "sliders.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::ChangeListener, public juce::Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void buttonClicked (juce::Button* button) override;
    void updateAngle1Delta();
    void updateAngle2Delta();
    void updateAngle3Delta();
    float synth();
    float osc();
    
private:
    //==============================================================================
    // gui segments
    Nav_Bar nBar;
    Filter_Menu fMenu;
    Oscillator_Menu oMenu;
    amplifier_Menu aMenu;
    juce::String test_string = "start";
    juce::TextButton save_button;
    juce::TextButton test_audio;
    juce::File save_directory;
    juce::Random random;
    bool audible;
    double sample_rate;
    double currentAngle1;
    double currentAngle2;
    double currentAngle3;
    double angleDelta1;
    double angleDelta2;
    double angleDelta3;
    double currentSample1;
    double currentSample2;
    double currentSample3;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
