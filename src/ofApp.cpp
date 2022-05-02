#include "ofApp.h"
#include <math.h>

using namespace glm;


//--------------------------------------------------------------
void ofApp::setup(){

	// print the available output ports to the console
	midiOut.listOutPorts();

    // connect
	midiOut.openPort(1);
	//mmidiOut.openPort("IAC Driver Pure Data In"); // by name
	//midiOut.openVirtualPort("ofxMidiOut"); // open a virtual port

    channel = 1;
	currentPgm = 0;
	note = 0;
	velocity = 0;
	pan = 0;
	bend = 0;
	touch = 0;
	polytouch = 0;
    page = 0;
    result = 0;


    rightGlove.setup();
    debugMode = true;

    ofSetFrameRate(30);

    //3D graphics setup
    ofEnableNormalizedTexCoords();
    material.setDiffuseColor(ofColor(255, 0, 0));
    
    light.setPointLight();
    light.setDiffuseColor(ofColor(255));
    light.setPosition(500, 0, 300);
    
    box.setPosition(-50,50,0);
    box.set(200, 100, 100);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    rightGlove.update();

    if(rightGlove.pageToggle) {
        // cout<<"pitch is " << pitch<<endl;
        //based on the pitch, choose a page
        //pitch is in degrees, 0 to 180
        if(rightGlove.pitch < -0.5) {
            page = 2;
        } else if (rightGlove.pitch >= -0.5 && rightGlove.pitch <= 0.5) {
            page = 1;
        } else if (rightGlove.pitch>0.5) {
            page = 0;
        }
    }

    if(rightGlove.gestureMode) {
            int cc = 0;
            int velocity = 0;
            int gestureSwitch;

            if(rightGlove.usingGlover) {
            gestureSwitch = rightGlove.gesture;
            } else {
                gestureSwitch = result-1;
            }

            if(!rightGlove.usingGlover){
            velocity  = ofMap(rightGlove.pitch, -1, 1, 127, 0);
            } else {
                velocity = rightGlove.pitch;
            }

            cout<<"velocity is " << pitch<<endl;
                        //slide roll to be at the bottom

            switch (gestureSwitch) {
                //finger point
                case 0:
                    //choose based on page
                    if(page == 0) {
                        // pitch
                        cc = 16;
                    } else if (page == 1) {
                        // portamento
                        cc = 22;
                    } else {
                        // expression pedal
                        cc = 4;
                    }
                    break;
                //fist
                case 1:
                    //control tempo
                    //choose based on page
                    if(page == 0) {
                        // filter
                        cc = 17;
                    } else if (page == 1) {
                        // filter type
                        cc = 23;
                    } else {
                        // env type
                        cc = 9;
                    }
                    break;
                // open hand
                case 2:
                    if(page == 0) {
                        // mix
                        cc = 18;
                    } else if (page == 1) {
                        // delay level
                        cc = 24;
                    } else {
                        // tempo
                        cc = 15;
                    }
                    break;
                // OK hand
                case 3:
                    if(page == 0) {
                        // sustain
                        cc = 19;
                    } else if (page == 1) {
                        // ring mod
                        cc = 25;
                    } else {
                        // synth mode
                        cc = 29;
                    }
                    break;
                // climber
                case 4:
                    if (page == 0) {
                        // filter env
                        cc = 20;
                    } else if (page == 1) {
                        // filter bandwidth
                        cc = 26;
                    } else {
                        // channel
                        cc = 127;
                    }
                    break;
                // pupper hand
                case 5:
                    if (page == 0) {
                        //modulation
                        cc = 21;
                    } else if (page == 1) {
                        // delay feedback
                        cc = 27;
                    } else {
                        // bypass
                        cc = 14;
                    }
                    break;
                default:
                    break;

            }
            cout<<cc<<endl;
            // cout<<"velocity"<<velocity<<endl;
            if(cc == 127) {
                // code that switches the midi channel    
            } else {
                midiOut.sendControlChange(channel, cc, velocity);
            }
    } 
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofSetColor(255);

  //-------RAPID-MIX---------------//
    std::vector<double> trainingInput;
    std::vector<double> trainingOutput;
    
    switch (recordingState) 
    {
        case 1:
            ofSetColor(255, 0, 0);
            break;
        case 2:
            ofSetColor(0, 255, 0);
            break;
        case 3:
            ofSetColor(0, 0, 255);
            break;
        case 4:
            ofSetColor(255, 0, 0);
            break;
        case 5:
            ofSetColor(0, 255, 0);
            break;
        case 6:
            ofSetColor(0, 0, 255);
            break;
        default:
            ofSetColor(255, 255, 255);
    }



    // we know the length is 8 because array 
    for (int j = 0; j < 8; j++)
    {
        trainingInput.push_back(rightGlove.bends[j]);
    }

    if(runToggle) {
        result = myKnn.run(trainingInput)[0];
    }

    if(debugMode) {
        rightGlove.draw(result);
    }

    
    if (recordingState) 
    {
        trainingOutput = { double(recordingState) };
        rapidLib::trainingExample tempExample;
        tempExample.input = trainingInput;
        tempExample.output = trainingOutput;
        myData.push_back(tempExample);
    }
    // begin 3D graphics
    ofEnableDepthTest();
    cam.begin();

    ofDrawAxis(100); // if you prefer a less busy visual helper

      //do all our 3D drawing here
    
    ofEnableLighting();
    
    light.enable();
    
    material.begin();

    glPushMatrix();
    if(rightGlove.usingGlover) {
        box.setOrientation(rightGlove.orientation);
    } else {
        box.setOrientation(rightGlove.quaternion);
    }
    box.draw();
    glPopMatrix();

    //disable everything
    material.end();
    light.disable();
    ofDisableLighting();
    cam.end();
    ofDisableDepthTest();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// ignore shift+s since it is used in keyReleased for sysex sending
	if(key == 'S') return;

    switch(key) {
        //1 key
        case 49:
            recordingState = 1;
            break;
        // 2 key
        case 50:
            recordingState = 2;
            break;
        // 3 key
        case 51:
            recordingState = 3;
            break;
        // 4 key
        case 52:
            recordingState = 4;
            break;
        // 5 key
        case 53:
            recordingState = 5;
            break;
        // 6 key
        case 54:
            recordingState = 6;
            break;
        // spacebar
        case 32:
            runToggle = (runToggle) ? false : true;
            if (!runToggle) {
                result = 0;
            }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	switch(key) {
	
		// send pgm change on arrow keys
		case OF_KEY_UP:
			currentPgm = (int) ofClamp(currentPgm+1, 0, 16);
			midiOut.sendProgramChange(channel, currentPgm);
            cout<<"program change"<< currentPgm<< endl;
			break;
		case OF_KEY_DOWN:
			currentPgm = (int) ofClamp(currentPgm-1, 0, 16);
			midiOut << ProgramChange(channel, currentPgm); // stream interface
			break;
		// print the port list
		case '?':
			midiOut.listOutPorts();
			break;
		default:
			break;
	}


    recordingState = 0;
    if (myData.size() > 0) myKnn.train(myData);
}
