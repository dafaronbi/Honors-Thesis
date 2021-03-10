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
    
    //current Angle at begining of phase
    currentAngle1 = 0;
    currentAngle2 = 0;
    currentAngle3 = 0;
    
    //current Sample at begining
    currentSample1 = 0;
    currentSample2 = 0;
    currentSample3 = 0;
    
    //silent at first
    audible = false;

    // Specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    nBar.removeChangeListener(this);
    save_button.removeListener(this);
    test_audio.removeListener(this);
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
        updateAngle1Delta();
        updateAngle2Delta();
        updateAngle3Delta();
        //fill buffer with random noise
        

        // Get a pointer to the start sample in the buffer for this audio output channel
        auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);

        // Fill the required number of samples with noise between -0.125 and +0.125
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample){
            auto currentSample = synth();
            leftBuffer[sample] = currentSample;
            rightBuffer[sample] = currentSample;
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
    
    auto nav_width = getWidth() < 160 ? getWidth() /3 : 160;
    
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
                    
                    // Get a pointer to the start sample in the buffer for this audio output channel
                    auto* leftBuffer  = write_buffer.getWritePointer (0, 0);
                    auto* rightBuffer = write_buffer.getWritePointer (1, 0);

                    // Fill the required number of samples with noise between -0.125 and +0.125
                    for (auto sample = 0; sample < write_buffer.getNumSamples(); ++sample){
                        auto currentSample = synth();
                        leftBuffer[sample] = currentSample;
                        rightBuffer[sample] = currentSample;
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
    

    void MainComponent::updateAngle1Delta()
        {
            auto cyclesPerSample = oMenu.osc1_frequency.getValue() / sample_rate;         // [2]
            angleDelta1 = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;          // [3]
        }

    void MainComponent::updateAngle2Delta()
    {
        auto cyclesPerSample = oMenu.osc2_frequency.getValue() / sample_rate;         // [2]
        angleDelta2 = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;          // [3]
    }

    void MainComponent::updateAngle3Delta()
    {
        auto cyclesPerSample = oMenu.osc3_frequency.getValue() / sample_rate;         // [2]
        angleDelta3 = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;          // [3]
    }

    float MainComponent::synth(){
        
        auto gain_total = juce::Decibels::decibelsToGain(aMenu.amp_total_gain.getValue());
        auto audio = osc();
        
        return (float) audio * gain_total;

    }

    float MainComponent::osc(){
        
        //update current angles
        currentAngle1 += angleDelta1;
        currentAngle2 += angleDelta2;
        currentAngle3 += angleDelta3;
        
        //update current samples
        currentSample1++;
        currentSample2++;
        currentSample3++;
        
        auto gain1 = juce::Decibels::decibelsToGain(oMenu.osc1_gain.getValue());
        auto gain2 = juce::Decibels::decibelsToGain(oMenu.osc2_gain.getValue());
        auto gain3 = juce::Decibels::decibelsToGain(oMenu.osc3_gain.getValue());
        
        float audio = 0;
        //add oscilator 1
        switch(oMenu.osc1_wav_shape.getSelectedId()){
            case 1:
                audio += std::sin (currentAngle1)*gain1;
                break;
            case 2:
                audio += (-2 / juce::MathConstants<double>::pi * std::atan(1 / std::tan(oMenu.osc1_frequency.getValue() *juce::MathConstants<double>::pi* currentSample1/sample_rate)))*gain1;
                break;
            case 3:
                audio += std::sin (currentAngle1) > 0 ? 1*gain1 : -1*gain1;
                break;
            case 4:
                audio += (2 / juce::MathConstants<double>::pi * std::asin(std::sin(currentAngle1)))*gain1;
                break;
            case 5:
                audio += (0.5*random.nextFloat()-1)*gain1;
        }
        
        
        //add oscilator 2
        switch(oMenu.osc2_wav_shape.getSelectedId()){
            case 1:
                audio += std::sin (currentAngle2)*gain2;
                break;
            case 2:
                audio += (-2 / juce::MathConstants<double>::pi * std::atan(1 / std::tan(oMenu.osc2_frequency.getValue() *juce::MathConstants<double>::pi* currentSample2/sample_rate)))*gain2;
                break;
            case 3:
                audio += std::sin (currentAngle2) > 0 ? 1*gain2 : -1*gain2;
                break;
            case 4:
                audio += (2 / juce::MathConstants<double>::pi * std::asin(std::sin(currentAngle2)))*gain2;
                break;
            case 5:
                audio += (0.5*random.nextFloat()-1)*gain2;
        }
        
        //add oscilator 3
        switch(oMenu.osc3_wav_shape.getSelectedId()){
            case 1:
                audio += std::sin (currentAngle3)*gain3;
                break;
            case 2:
                audio += (-2 / juce::MathConstants<double>::pi * std::atan(1 / std::tan(oMenu.osc3_frequency.getValue() *juce::MathConstants<double>::pi* currentSample3/sample_rate)))*gain3;
                break;
            case 3:
                audio += std::sin (currentAngle3) > 0 ? 1*gain3 : -1*gain3;
                break;
            case 4:
                audio += (2 / juce::MathConstants<double>::pi * std::asin(std::sin(currentAngle3)))*gain3;
                break;
            case 5:
                audio += (0.5*random.nextFloat()-1)*gain3;
        }
        return audio;
        
    }

