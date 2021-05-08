import os
import numpy as np
import tensorflow as tf
import librosa
import time
import json


#data to be saved
data = {"name":["435.23_440_445_-3_-3_-3_1_2_3_20000_20000_1_1_0_1_0_2_-10.wav"],
        "mapping":[["o_freq1", "o_freq2", "o_freq3", "o_shape1", "o_shape2", "o_shape3", "f_freq1",
        "f_freq2", "f_freq2", "f_type1", "f_type2", "a_attack", "a_decay", "a_sustain", "a_release"]],
        "features":[],
        "labels":[435.23,440,445,-3,-3,-3,1,2,3,20000,20000,1,1,0,1,0,2,-10]
}

#extract signal
signal, sr = librosa.load("435.23_440_445_-3_-3_-3_1_2_3_20000_20000_1_1_0_1_0_2_-10.wav",sr=44100)

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

#append feature vector
data["features"].extend(mfcc.tolist())
data["features"].extend(chroma.tolist())
data["features"].extend(spectral_centroid_features)
data["features"].extend(spectral_contrast_features)

print(data["features"])

with open("data.json", "w") as fp:
    json.dump(data,fp, indent=4)
