/*
  ==============================================================================

    header.h
    Created: 29 Mar 2021 5:09:18pm
    Author:  Daniel Faronbi

  ==============================================================================
*/
#include <JuceHeader.h>
#include <filesystem>

#pragma once

class audio_generate{
    public:
    //==============================================================================
    audio_generate(double, double, double, double, double, double, int, int, int, double, double, int, int, double, double, double, double,double, juce::String);
    ~audio_generate();

    void updateAngle1Delta();
    void updateAngle2Delta();
    void updateAngle3Delta();
    void updateFilter1Values();
    void updateFilter2Values();
    void updateEnvelopes();
    float synth();
    float osc();
    void save_file();
    
private:
    //==============================================================================
    juce::File save_directory;
    juce::Random random;
    double sample_rate;
    
    //current Angle
    double currentAngle1;
    double currentAngle2;
    double currentAngle3;
    
    //current frequency of oscillators
    double currentFrequency1;
    double currentFrequency2;
    double currentFrequency3;
    
    //oscillator gains
    double o1_gain;
    double o2_gain;
    double o3_gain;
    
    //oscillator shape
    int o1_shape;
    int o2_shape;
    int o3_shape;
    
    //Angle to update for  desired frequency
    double angleDelta1;
    double angleDelta2;
    double angleDelta3;
    
    //current  sample
    double currentSample1;
    double currentSample2;
    double currentSample3;
    
    //filter variables
    double cutoff_freq1;
    double cutoff_freq2;
    double f_attack;
    double f_decay;
    double f_sustain;
    double f_release;
    int filter1_type;
    int filter2_type;
    
    //IIR filter variables
    juce::IIRFilter filter1;
    juce::IIRFilter filter2;
    
    //amplifier variables
    double a_total_gain;
    double a_attack;
    double a_decay;
    double a_sustain;
    double a_release;
    
    //ASDRs
    juce::ADSR f_adsr;
    juce::ADSR a_adsr;
};

int main (int, char**);
