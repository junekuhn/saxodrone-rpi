# Saxodrone + MiMU Glove

This OpenFrameworks app allows me to use the MiMU glove to modulate my Meris guitar pedals.  This is a cross-platform application, but designed for the Raspberry Pi.

Use the number keys 1-6 to train gestures. Hit the spacebar to run the neural net.

### testing interactions

to make sure everything works before getting out the glove and saxophone, we can test using OSC from a phone.  we can use orientation from the phone, trigger 1-6 gestures, emulate the button, etc.  

### performance / dev modes reference

DEVELOPMENT MODE

toggle dev/perform modes - d key

6 gestures - fist, open hand, one finger point, climber, puppet, ok
page 1 - pitch, filter, mix, sus, env, mod
page 2 - alt params
page 3 - exp, env type, tempo, synth mode, channel?, program?

if the button is held down - you select which page
when the button is released - that toggles gesture mode

page 0 - up
page 1 - flat
page 2 - down

PERFORMANCE MODE  

1 - finger point - mix - cc18
2 - fist - preset selection - program change 1-6
3 - open hand - exp - cc4
4 - ok - delay feedback - cc27
5 - climber - delay level - cc24
6 - puppet hand - portamento - cc22



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

