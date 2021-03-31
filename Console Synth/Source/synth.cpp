/*
  ==============================================================================

    synth.cpp
    Created: 29 Mar 2021 5:11:14pm
    Author:  Daniel Faronbi

  ==============================================================================
*/

#include "header.h"

audio_generate::audio_generate(double o_f1, double o_f2, double o_f3, double o_g1, double o_g2, double o_g3, int o_s1, int o_s2, int o_s3, double cf1, double cf2, int f1_t, int f2_t, double aa, double ad, double as, double ar, double a_tg, juce::String output_file)
{
    
    //current Angle at begining of phase
    currentAngle1 = 0;
    currentAngle2 = 0;
    currentAngle3 = 0;
    
    //current Sample at begining
    currentSample1 = 0;
    currentSample2 = 0;
    currentSample3 = 0;
    
    //oscillator parameters
    currentFrequency1 = o_f1;
    currentFrequency2 = o_f2;
    currentFrequency3 = o_f3;
    
    o1_gain = o_g1;
    o2_gain = o_g2;
    o3_gain = o_g3;
    
    o1_shape = o_s1;
    o2_shape = o_s2;
    o3_shape = o_s3;
    
    //set sample rate
    sample_rate = 441000;
    

    //initialize synth variables
    cutoff_freq1 = cf1;
    cutoff_freq2 =  cf2;
    filter1_type = f1_t;
    filter2_type = f2_t;
    a_attack = aa;
    a_decay = ad;
    a_sustain = as;
    a_release = ar;
    a_total_gain = a_tg;
    
    updateAngle1Delta();
    updateAngle2Delta();
    updateAngle3Delta();
    
    updateFilter1Values();
    updateFilter2Values();
    updateEnvelopes();
    
    save_directory = juce::File(juce::File::getCurrentWorkingDirectory().getFullPathName() + "/" + output_file);
}

audio_generate::~audio_generate()
{
    
}
    

    void audio_generate::updateAngle1Delta()
        {
            auto cyclesPerSample = currentFrequency1 / sample_rate;         // [2]
            angleDelta1 = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;          // [3]
        }

    void audio_generate::updateAngle2Delta()
    {
        auto cyclesPerSample = currentFrequency2 / sample_rate;         // [2]
        angleDelta2 = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;          // [3]
    }

    void audio_generate::updateAngle3Delta()
    {
        auto cyclesPerSample = currentFrequency3 / sample_rate;         // [2]
        angleDelta3 = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;          // [3]
    }

    void audio_generate::updateFilter1Values(){
        
        //get cuttoff frequency from envelope
        double cutoff = cutoff_freq1;
        
        //select filter type and get coefficients
        switch(filter1_type){
            case 1:
                filter1.setCoefficients(juce::IIRCoefficients::makeLowPass(sample_rate, cutoff));
                break;
            case 2:
                filter1.setCoefficients(juce::IIRCoefficients::makeHighPass(sample_rate, cutoff));
                break;
            case 3:
                filter1.setCoefficients(juce::IIRCoefficients::makeBandPass(sample_rate, cutoff));
                break;
            case 4:
                filter1.setCoefficients(juce::IIRCoefficients::makeNotchFilter(sample_rate, cutoff));
                break;
            default:
                filter1.setCoefficients(juce::IIRCoefficients::makeLowPass(sample_rate, cutoff));
            
        }
        
        filter1.reset();
    }

    void audio_generate::updateFilter2Values(){
        
        //get cuttoff frequency from envelope
        double cutoff = cutoff_freq2;
        
        //select filter type and get coefficients
        switch(filter2_type){
            case 1:
                filter2.setCoefficients(juce::IIRCoefficients::makeLowPass(sample_rate, cutoff));
                break;
            case 2:
                filter2.setCoefficients(juce::IIRCoefficients::makeHighPass(sample_rate, cutoff));
                break;
            case 3:
                filter2.setCoefficients(juce::IIRCoefficients::makeBandPass(sample_rate, cutoff));
                break;
            case 4:
                filter2.setCoefficients(juce::IIRCoefficients::makeNotchFilter(sample_rate, cutoff));
                break;
            default:
                filter2.setCoefficients(juce::IIRCoefficients::makeLowPass(sample_rate, cutoff));
        }
            
        filter2.reset();
    }

    void audio_generate::updateEnvelopes(){
        juce::ADSR::Parameters fParams;
        juce::ADSR::Parameters aParams;
        
        fParams.attack = f_attack;
        fParams.decay = f_decay;
        fParams.sustain = f_sustain;
        fParams.release = f_release;
        
        aParams.attack = a_attack;
        aParams.decay = a_decay;
        aParams.sustain = a_sustain;
        aParams.release = a_release;
        
        f_adsr.setParameters(fParams);
        a_adsr.setParameters(aParams);
    }

    float audio_generate::synth(){
        
        //calculate gain from final gain  slider
        auto gain_total = juce::Decibels::decibelsToGain(a_total_gain);
        
        //generate audio  from oscillator
        auto audio = osc();
        
        //apply filter1
        audio = filter1.processSingleSampleRaw(audio);
        
        //apply filter2
        audio = filter2.processSingleSampleRaw(audio);
        
        //add adsr
        audio *= a_adsr.getNextSample();
        
        return (float) audio * gain_total;

    }

    float audio_generate::osc(){
        
        //update current angles
        currentAngle1 += angleDelta1;
        currentAngle2 += angleDelta2;
        currentAngle3 += angleDelta3;
        
        //update current samples
        currentSample1++;
        currentSample2++;
        currentSample3++;
        
        auto gain1 = juce::Decibels::decibelsToGain(o1_gain);
        auto gain2 = juce::Decibels::decibelsToGain(o2_gain);
        auto gain3 = juce::Decibels::decibelsToGain(o3_gain);
        
        float audio = 0;
        //add oscilator 1
        switch(o1_shape){
            case 1:
                audio += std::sin (currentAngle1)*gain1;
                break;
            case 2:
                audio += (-2 / juce::MathConstants<double>::pi * std::atan(1 / std::tan(currentFrequency1 *juce::MathConstants<double>::pi* currentSample1/sample_rate)))*gain1;
                break;
            case 3:
                audio += std::sin (currentAngle1) > 0 ? 1*gain1 : -1*gain1;
                break;
            case 4:
                audio += (2 / juce::MathConstants<double>::pi * std::asin(std::sin(currentAngle1)))*gain1;
                break;
            case 5:
                audio += (2*random.nextFloat()-1)*gain1;
        }
        
        
        //add oscilator 2
        switch(o2_shape){
            case 1:
                audio += std::sin (currentAngle2)*gain2;
                break;
            case 2:
                audio += (-2 / juce::MathConstants<double>::pi * std::atan(1 / std::tan(currentFrequency2 *juce::MathConstants<double>::pi* currentSample2/sample_rate)))*gain2;
                break;
            case 3:
                audio += std::sin (currentAngle2) > 0 ? 1*gain2 : -1*gain2;
                break;
            case 4:
                audio += (2 / juce::MathConstants<double>::pi * std::asin(std::sin(currentAngle2)))*gain2;
                break;
            case 5:
                audio += (2*random.nextFloat()-1)*gain2;
        }
        
        //add oscilator 3
        switch(o3_shape){
            case 1:
                audio += std::sin (currentAngle3)*gain3;
                break;
            case 2:
                audio += (-2 / juce::MathConstants<double>::pi * std::atan(1 / std::tan(currentFrequency3 *juce::MathConstants<double>::pi* currentSample3/sample_rate)))*gain3;
                break;
            case 3:
                audio += std::sin (currentAngle3) > 0 ? 1*gain3 : -1*gain3;
                break;
            case 4:
                audio += (2 / juce::MathConstants<double>::pi * std::asin(std::sin(currentAngle3)))*gain3;
                break;
            case 5:
                audio += (2*random.nextFloat()-1)*gain3;
        }
        return audio;
        
    }

    void audio_generate::save_file(){
            
            //10 second buffer initialize
            juce::AudioBuffer<float> write_buffer;
            write_buffer.setSize(2,sample_rate*5);
            
            // Get a pointer to the start sample in the buffer for this audio output channel
            auto* leftBuffer  = write_buffer.getWritePointer (0, 0);
            auto* rightBuffer = write_buffer.getWritePointer (1, 0);
            
            //start envelope attack
            f_adsr.noteOn();
            a_adsr.noteOn();

            // Fill the required number of samples with noise between -0.125 and +0.125
            for (auto sample = 0; sample < write_buffer.getNumSamples()/2; ++sample){
                auto currentSample = synth();
                leftBuffer[sample] = currentSample;
                rightBuffer[sample] = currentSample;
            }
            
            //start envelope release
            f_adsr.noteOff();
            a_adsr.noteOff();
            
            for (auto sample = write_buffer.getNumSamples()/2; sample < write_buffer.getNumSamples(); ++sample){
                auto currentSample = synth();
                leftBuffer[sample] = currentSample;
                rightBuffer[sample] = currentSample;
            }
//            //delete file
            save_directory.deleteFile();
        
            //write to file
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
