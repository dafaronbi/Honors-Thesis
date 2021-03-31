/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "header.h"


//==============================================================================
int main (int argc, char* argv[])
{
    //initialize variables for synthesizer
    double o1_freq;
    double o2_freq;
    double o3_freq;
    double o1_gain;
    double o2_gain;
    double o3_gain;
    int o1_type;
    int o2_type;
    int o3_type;
    double f1_cuttoff;
    double f2_cuttoff;
    int f1_type;
    int f2_type;
    double a_attack;
    double a_decay;
    double a_sustain;
    double a_release;
    double a_total_gain;
    juce::String output_file;
    
    //save parameters from command line
    if(argc > 19){
        o1_freq = atof(argv[1]);
        o2_freq = atof(argv[2]);
        o3_freq = atof(argv[3]);
        o1_gain = atof(argv[4]);
        o2_gain = atof(argv[5]);
        o3_gain = atof(argv[6]);
        o1_type = atoi(argv[7]);
        o2_type = atoi(argv[8]);
        o3_type = atoi(argv[9]);
        f1_cuttoff = atof(argv[10]);
        f2_cuttoff = atof(argv[11]);
        f1_type = atoi(argv[12]);
        f2_type = atoi(argv[13]);
        a_attack = atof(argv[14]);
        a_decay = atof(argv[15]);
        a_sustain = atof(argv[16]);
        a_release = atof(argv[17]);
        a_total_gain = atof(argv[18]);
        output_file = argv[19];
        
        audio_generate synthesizer(o1_freq, o2_freq, o3_freq, o1_gain, o2_gain, o3_gain, o1_type, o2_type, o3_type, f1_cuttoff, f2_cuttoff, f1_type, f2_type, a_attack, a_decay, a_sustain, a_release, a_total_gain, output_file);
        
        synthesizer.save_file();
        
    }
    else{
        std::cout << "PLEASE INPUT AN APPROPRIATE NUMBER OF PARAMETERS\n";
    }

    return 0;
}
