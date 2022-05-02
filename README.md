# Saxodrone + MiMU Glove

This OpenFrameworks app allows me to use the MiMU glove to modulate my Meris guitar pedals.  This is a cross-platform application, but designed for the Raspberry Pi.

Use the number keys 1-6 to train gestures. Hit the spacebar to run the neural net.

### setup

Download the following add-ons: ofxRapidLib, ofxMidi, and ofxOsc
clone this repository in myApps folder

### build

navigate to the top level folder
`make` 

#### run 

`make RunRelease`


RapidLib example code
https://github.com/mzed/ofxRapidLib/blob/main/example-objectClassification/src/ofApp.cpp

