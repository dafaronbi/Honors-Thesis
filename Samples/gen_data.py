`   import os
import tensorflow as tf
import librosa
import time
import json
import numpy as np

cmd = "./Console\ Synth "
path = "data/"

#data to be saved
data = {"array": []
}

#frequencies from c3- c6
frequency_list = [440.00, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61]

o_freq2 = 440
o_freq3 = 440

o_shape2 = 1
o_shape3 = 1

for o_freq1 in frequency_list:
    for o_shape1 in range(1,6):
        for f_freq1 in range(200,15000,10000):
            for f_freq2 in range(200,15000,10000):
                for f_type1 in range(1,5):
                    for f_type2 in range(1,5):
                        for a_attack in range(0,10,5):
                            for a_decay in range(0,10,5):
                                for a_sustain in range(5,10,2):
                                    for a_release in range(0,10,5):
                                            
                                        #name the file and add parameters before creating audio
                                        cmd += str(o_freq1) + " "
                                        name = str(o_freq1) + "_"
                                        cmd += str(440) + " "
                                        name += str(440) + "_"
                                        cmd += str(440) + " "
                                        name += str(440) + "_"
                                        cmd += str(0) + " "
                                        name += str(0) + "_"
                                        cmd += str(-100) + " "
                                        name += str(-100) + "_"
                                        cmd += str(-100) + " "
                                        name += str(-100) + "_"
                                        cmd += str(o_shape1) + " "
                                        name += str(o_shape1) + "_"
                                        cmd += str(1) + " "
                                        name += str(1) + "_"
                                        cmd += str(1) + " "
                                        name += str(1) + "_"
                                        cmd += str(f_freq1) + " "
                                        name += str(f_freq1) + "_"
                                        cmd += str(f_freq2) + " "
                                        name += str(f_freq2) + "_"
                                        cmd += str(f_type1) + " "
                                        name += str(f_type1) + "_"
                                        cmd += str(f_type2) + " "
                                        name += str(f_type2) + "_"
                                        cmd += str(0.1*a_attack) + " "
                                        name += str(0.1*a_attack) + "_"
                                        cmd += str(0.1*a_decay) + " "
                                        name += str(0.1*a_decay) + "_"
                                        cmd += str(0.1*a_sustain) + " "
                                        name += str(0.1*a_sustain) + "_"
                                        cmd += str(0.1*a_release) + " "
                                        name += str(0.1*a_release) + "_"
                                        cmd += str(0) + " "
                                        name += str(0)
                                        name += ".wav"
                                        print(name)
                                        cmd += path+name
                                        
                                        #generate audio
                                        os.system(cmd)
                                        
                                        #extract features from audio
                                        #extract signal
                                        signal, sr = librosa.load(path+name,sr=44100)

                                        #generate mfcc's
                                        mfcc = librosa.feature.mfcc(signal,sr=sr,n_mfcc=10)
                                        mfcc = np.mean(mfcc, axis=1)

                                        #generate chroma features
                                        chroma = librosa.feature.chroma_stft(signal, sr=sr)
                                        chroma = np.mean(chroma, axis=1)

                                        #generate spectral centroid features
                                        spectral_centroid = librosa.feature.spectral_centroid(signal, sr=sr)
                                        spectral_centroid_features = [np.mean(spectral_centroid[0]), np.std(spectral_centroid[0]), np.max(spectral_centroid[0]), np.min(spectral_centroid[0])]

                                        #generate spectral contrast features
                                        spectral_constrast = librosa.feature.spectral_contrast(signal, sr=sr)
                                        spectral_contrast_features = [np.mean(spectral_constrast[0]), np.std(spectral_constrast[0]), np.max(spectral_constrast[0]), np.min(spectral_constrast[0])]
                                        
                                        features = []
                                        features.extend(mfcc.tolist())
                                        features.extend(chroma.tolist())
                                        features.extend(spectral_centroid_features)
                                        features.extend(spectral_contrast_features)
                                        
                                        data["array"].append({"name":name,
                                        "mapping":[["o_freq1", "o_freq2", "o_freq3", "o_shape1", "o_shape2", "o_shape3", "f_freq1",
                                        "f_freq2", "f_freq2", "f_type1", "f_type2", "a_attack", "a_decay", "a_sustain", "a_release"]],
                                        "features":features,
                                        "labels":[o_freq1,o_freq2,o_freq3,o_shape1,o_shape2,o_shape3,f_freq1,f_freq2,f_type1,f_type2,0.1*a_attack,0.1*a_decay,0.1*a_sustain,0.1*a_release]})
                                        
                                        with open(path+"data.json", "w") as fp:
                                            json.dump(data,fp, indent=4)
                                        
                                        
                                        os.system("rm " + path+name);
                                                                                        
                                        #reset command name
                                        cmd = "./Console\ Synth "
                                                        
                                                        
                                                        
