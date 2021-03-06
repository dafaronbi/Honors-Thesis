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
    addAndMakeVisible(save_button);
    addAndMakeVisible(test_audio);
    
    //only oscilattior menu is on by default
    oMenu.setVisible(true);
    
    //listen to navbar changes
    nBar.addChangeListener(this);
    
    //configure button text
    save_button.setButtonText ("Save Sample");
    test_audio.setButtonText("Test Audio");
    
    //add listener for button
    save_button.addListener(this);
    test_audio.addListener(this);
    
    //set default window size
    setSize(800, 600);
    
    //silent at first
    audible = false;

    // Specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);
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
    juce::String message;
    message << "Preparing to play audio...\n";
    message << " samplesPerBlockExpected = " << samplesPerBlockExpected << "\n";
    message << " sampleRate = " << sampleRate;
    juce::Logger::getCurrentLogger()->writeToLog (message);
    sample_rate = sampleRate;
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    //play audio when button is pressed
    if(audible){
        //fill buffer with random noise
     for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            // Get a pointer to the start sample in the buffer for this audio output channel
            auto* buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
 
            // Fill the required number of samples with noise between -0.125 and +0.125
            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample){
                buffer[sample] = random.nextFloat() * 0.1f - 0.125f;
//
            }
        }
    }
    else{
        
        bufferToFill.clearActiveBufferRegion();
        
    }
    
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
    
    auto nav_width = getWidth() < 175 ? getWidth() /3 : 175;
    
    //set areas of things
    nBar.setBounds(area.removeFromLeft(nav_width));
    auto button_area = area.removeFromBottom(getHeight()/15);
    save_button.setBounds(button_area.removeFromRight(button_area.getWidth()/2));
    test_audio.setBounds(button_area);
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

void MainComponent::buttonClicked (juce::Button* button)
    {
        if (button == &test_audio)
        {
            //make audio audible
            audible = true;
            
            //wait ten seconds
            juce::Time::waitForMillisecondCounter(juce::Time::getMillisecondCounter() + 1000);
            
            audible = false;
        }
        
        if(button == &save_button)
        {
            
            juce::FileChooser myChooser ("Select an output file",
                           juce::File::getSpecialLocation (juce::File::userHomeDirectory),
                           "*.wav");
            
            if (myChooser.browseForFileToSave(true))
                {
                    
                    save_directory = myChooser.getResult();
                    
                    
                    //10 second buffer initialize
                    juce::AudioBuffer<float> write_buffer;
                    write_buffer.setSize(2,sample_rate*10);
                    
                    for (int channel = 0; channel < write_buffer.getNumChannels(); ++channel)
                            {
                                // Get a pointer to the start sample in the buffer for this audio output channel
                                auto* buffer = write_buffer.getWritePointer (channel, 0);

                                // Fill the required number of samples with noise between -0.125 and +0.125
                                for (auto sample = 0; sample < write_buffer.getNumSamples(); ++sample){
                                    buffer[sample] = random.nextFloat() * 0.1f - 0.125f;

                                }
                            }
        
                    juce::WavAudioFormat format;
                    std::unique_ptr<juce::AudioFormatWriter> writer;
                    writer.reset (format.createWriterFor (new juce::FileOutputStream (save_directory),
                                                          (int)sample_rate,
                                                          write_buffer.getNumChannels(),
                                                          24,
                                                          {},
                                                          0));
                    if (writer != nullptr)
                        writer->writeFromAudioSampleBuffer (write_buffer, 0, write_buffer.getNumSamples());
                    
                }
            
        }
    }
