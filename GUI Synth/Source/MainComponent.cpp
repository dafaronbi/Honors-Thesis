#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    
    //add child components
    addChildComponent(oMenu);
    addChildComponent(fMenu);
    addChildComponent(aMenu);
    
    addAndMakeVisible(nBar);
    
    //only oscilattior menu is on by default
    oMenu.setVisible(true);
    
    //listen to navbar changes
    nBar.addChangeListener(this);
    
    //set default window size
    setSize(800, 600);

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
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    nBar.removeChangeListener(this);
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::black);

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    //get the local area
    auto area = getLocalBounds();
    
    auto nav_width = getWidth() < 150 ? getWidth() /3 : 150;
    
    //set areas of things
    nBar.setBounds(area.removeFromLeft(nav_width));
    fMenu.setBounds(area);
    oMenu.setBounds(area);
    aMenu.setBounds(area);
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        oMenu.setVisible(false);
        fMenu.setVisible(false);
        aMenu.setVisible(false);
        if(source == &nBar){
            switch(nBar.get_button_click()){
                case 0:
                    oMenu.setVisible(true);
                    break;
                case 1:
                    fMenu.setVisible(true);
                    break;
                case 2:
                    aMenu.setVisible(true);
                    break;
            };
        }
        repaint();
    }
