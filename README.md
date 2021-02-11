# Honors-Thesis
Synthesizer Parameter Approximation by Deep Learning

Project for honors thesis spring 2021

## Abstract
Synthesizers have been an essential tool for composers of any style of music including computer generated sound. They allow for an expansion in timbral variety to the orchestration of a piece of music or sound scape. Sound designers are trained to be able to recreate a timbre in their head using a synthesizer. This works well for simple sounds but becomes more difficult as the number of parameters required to produce a specific timbre increase. The goal of this research project is to formulate a method for synthesizers to approximate a timbre given an input audio sample using deep learning. The synthesizer should be able to modify its settings (oscillators, filters, LFOs, effects, etc.) to produce an audio signal as close to the input sample as possible. A cost function will measure the difference between the outputted audio signal from the learned synthesizer parameters and the original audio signal that is being mimicked.


### Phase 1
Synthesizer Creation with JUCE Framework: https://juce.com/
The synthesizer will be a command line application that generates samples

### Phase 2
Data set generation.
A variety of audio samples will be generated to input and test the deep learning network.

### Phase 3
Deep learning module
The deep learning module will be created with tensor flow
