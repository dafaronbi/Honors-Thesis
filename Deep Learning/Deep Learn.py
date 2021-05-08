import os
import numpy as np
import tensorflow.keras as keras
import json
from sklearn.model_selection import train_test_split


#load data
with open("data.json","r") as fp:
    data_in = json.load(fp)

#arrays to store features and labels
features = list()
labels = list()

#get the array of all data
data = data_in["array"]

#sift through data and add elements
for element in data:
    features.append(element["features"])
    labels.append(element["labels"])
    
#make into numpy array
features = np.array(features)
labels = np.array(labels)

#get split data for training and testing
inputs_train, inputs_test, outputs_train, outputs_test = train_test_split(features,labels, test_size=0.3)

#build neural network arcitecture
model = keras.Sequential([
    #input layer
    keras.layers.Dense(30, activation="relu", name="input",input_shape=(30,)),
    
    #hidden layers
    keras.layers.Dense(1024, activation="relu", name="hiddenLayer1"),
    keras.layers.Dense(512, activation="relu", name="hiddenLayer2"),
    
    #output layers
    keras.layers.Dense(14, activation="softmax", name="outputLayer")
    ])
    
optimizer = keras.optimizers.Adam(learning_rate=0.01)

model.compile(optimizer=optimizer,loss="categorical_crossentropy", metrics= ["accuracy"])

model.summary()

#train
model.fit(inputs_train,outputs_train, validation_data=(inputs_test,outputs_test), epochs =20, batch_size = 32)

    
