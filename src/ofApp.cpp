#include "ofApp.h"
#include <math.h>

using namespace glm;

//todo
/*
figure out how to save and load the gesture data
 - save the trained model
 - each example is a vector of doubles
 - this may be difficult
set it up on rpi
- display and keyboard and mouse
- 
playtest with saxophone
set up channel switching - optional
set up mercury 7 controls

- graphics
    - texture the cube
    - add a shader or some background
    - 

// mapping ideas
holding the button down - which page
changing cc params
6 gestures - fist, open hand, one finger point, climber, puppet, two finger point
page 1 - pitch, filter, mix, sus, env, mod
page 2 - alt params
page 3 - exp, env type, tempo, synth mode, channel?, program?

change channel, what is the cc going to be ???
haptic feedback


refactoring!!

 - glove stuff to its own class (osc)
 - midi stuff to its own class? maybe

 testing notes
  - a lot of parameters do not want to have glove interaction
  - reaching the button might require two hands depending on pose
  - pitch seems the most reliable 
  - puppet hand and open hand are often confused
  - i need to show on the gui what parameter I'm modifying
  - to change it to raspberry pi /wifi/send/ip "192.168.0.179"
  - to change it to mac (use glover or) /wifi/send/ip "192.168.0.243"
  - the glove ip is 192.168.1.99



*/

//--------------------------------------------------------------
void ofApp::setup(){

	// print the available output ports to the console
	midiOut.listOutPorts();

    // connect
	if(isRPI) {
		midiOut.openPort(1);
	} else {
		midiOut.openPort(0); // by number
	}
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

    orientation = vec3(0.,0.,0.);
    accel = vec3(0., 0., 0.);
    gyro = vec3(0., 0., 0.);
    magnet = vec3(0., 0., 0.);
    gestureMode = false;
    pageToggle = false;
    gesture = 0;
    page = 0;

    if(usingGlover) {
        rx.setup(GLOVER);
    } else {
        rx.setup(GLOVE);
    }

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
    // check for waiting messages
	while(rx.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		rx.getNextMessage(m);

        if(usingGlover) {
            // switch for incoming Glover messages
            if(m.getAddress() == "/roll"){
                // cout<<m.getArgAsFloat(0)<<endl;
                orientation.x = m.getArgAsFloat(0);
            }
            // check for mouse button message
            // x is roll
            // y is pitch
            // z is yaw
            else if(m.getAddress() == "/pitch"){
                orientation.z = m.getArgAsFloat(0);
            } else if (m.getAddress() == "/yaw") {
                orientation.y = m.getArgAsFloat(0);    
            }
             else if (m.getAddress() == "/button") {
                gestureMode = m.getArgAsBool(0);  
                // cout<<gestureMode<<endl; 
            } else if (m.getAddress() == "/gesture") {
                gesture = m.getArgAsInt(0);
            }
            else {
                // unrecognized message: display on the bottom of the screen
                // messageBuffer.push_front(m.getAddress() + ": UNRECOGNIZED MESSAGE");
            }
        //using the glove without glover
        } else {
            if(m.getAddress() == "/glove/button") {
                 buttonState = m.getArgAsFloat(0);
                 //toggle gesturemode
                if(!buttonState) {
                    gestureMode = !gestureMode;
                    pageToggle = false;
                    cout<< "changing to page " << page << endl;

                } else {
                    //this means the button is held down
                     pageToggle = true;
                }
            } 
            else if (m.getAddress() == "/quaternion") {
                quaternion.w = m.getArgAsFloat(0);
                quaternion.x = m.getArgAsFloat(1);
                quaternion.y = m.getArgAsFloat(2);
                quaternion.z = m.getArgAsFloat(3);
            }
            else if (m.getAddress() == "/glove/bend") {
                for(int i = 0; i<8; i++) {
                    bends[i] = m.getArgAsFloat(i);
                }
            }
            else if (m.getAddress() == "/sensors") {
                gyro.x = m.getArgAsFloat(0);
                gyro.y = m.getArgAsFloat(1);
                gyro.z = m.getArgAsFloat(2);

                accel.x = m.getArgAsFloat(3);
                accel.y = m.getArgAsFloat(4);
                accel.z = m.getArgAsFloat(5);

                magnet.x = m.getArgAsFloat(6);
                magnet.y = m.getArgAsFloat(7);
                magnet.z = m.getArgAsFloat(8);

                baro = m.getArgAsFloat(9);
                
            }
             else {
                // cout<<m.getArgAsFloat(0)<<endl;
            }
        }
		
    }

    if(usingGlover) {
        roll = orientation.x;
        pitch = orientation.y;
    } else {
        // pitch = eulerAngles(quaternion).z;
        // //this shouldn't be yaw, but it is :(
        // yaw = eulerAngles(quaternion).x;
        // //this shouldn't be pitch, but it is :(
        // roll = eulerAngles(quaternion).y;

        // from 
        // yaw = accel.x;
        //from -1 to 1
        pitch = accel.x;
        //from -1 to 1
        roll = accel.z;
    }
 


    if(pageToggle) {
        // cout<<"pitch is " << pitch<<endl;
        //based on the pitch, choose a page
        //pitch is in degrees, 0 to 180
        if(pitch < -0.5) {
            page = 2;
        } else if (pitch >= -0.5 && pitch <= 0.5) {
            page = 1;
        } else if (pitch>0.5) {
            page = 0;
        }
    }

    if(gestureMode) {
            int cc = 0;
            int velocity = 0;
            int gestureSwitch;

            if(usingGlover) {
            gestureSwitch = gesture;
                
            } else {
                gestureSwitch = result-1;
            }

            velocity = ofMap(pitch, -1, 1, 127, 0);

            cout<<"velocity is " << velocity<<endl;
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

  if(gestureMode) {
    ofDrawBitmapString("Gesture Mode Enabled", ofGetWidth()/2, 50);
  } else {
    ofDrawBitmapString("Gesture Mode Disabled", ofGetWidth()/2, 50);
  }

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
        trainingInput.push_back(bends[j]);
    }


    if(usingGlover) {
        ofDrawBitmapString("Using Glover", ofGetWidth()/2, 10);
        ofDrawBitmapString(gestureLookup(gesture), ofGetWidth()/2, 30);
    } else {
        ofDrawBitmapString("No Glovers Allowed", ofGetWidth()/2, 10);
        if(runToggle) {
            result = myKnn.run(trainingInput)[0];

            ofDrawBitmapString(gestureLookup(result-1), ofGetWidth()/2, 30);
        }
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
    if(usingGlover) {
        box.setOrientation(orientation);
    } else {
        box.setOrientation(quaternion);
        box.setScale(magnet/100);
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

string ofApp::gestureLookup(int gesture) {

    string result;
    switch(gesture) {
        case 0:
            result = "Finger Point";
            break;
        case 1:
            result = "Fist";
            break;
        case 2:
            result = "Open Hand";
            break;
        case 3:
            result = "OK";
            break;
        case 4:
            result = "Climber";
            break;
        case 5:
            result = "Puppet Hand";
            break;
        default:
            result = "unrecognized gesture";
            break;
    }

    return result;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// ignore shift+s since it is used in keyReleased for sysex sending
	if(key == 'S') return;

	// send a note on if the key is a letter or a number
	if(isalnum((unsigned char) key)) {
	
		// scale the ascii values to midi velocity range 0-127
		// see an ascii table: http://www.asciitable.com/
		note = ofMap(key, 48, 122, 0, 127);
		velocity = 64;
		midiOut.sendNoteOn(channel, note,  velocity);
		
		// print out both the midi note and the frequency
		ofLogNotice() << "note: " << note
		              << " freq: " << ofxMidi::mtof(note) << " Hz";
	}

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
    // std::cout << "runToggle " << runToggle << std::endl;
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

		// aftertouch
		case '[':
			touch = 64;
			midiOut.sendAftertouch(channel, touch);
			break;
		case ']':
			touch = 127;
			midiOut << Aftertouch(channel, touch); // stream interface
			break;

		// poly aftertouch
		case '<':
			polytouch = 64;
			midiOut.sendPolyAftertouch(channel, 64, polytouch);
			break;
		case '>':
			polytouch = 127;
			midiOut << PolyAftertouch(channel, 64, polytouch); // stream interface
			break;
			
		// sysex using raw bytes (use shift + s)
		case 'S': {
			// send a pitch change to Part 1 of a MULTI on an Akai sampler
			// from http://troywoodfield.tripod.com/sysex.html
			//
			// do you have an S2000 to try?
			//
			// note: this is probably not as efficient as the next two methods
			//       since it sends only one byte at a time, instead of all
			//       at once
			//
			midiOut.sendMidiByte(MIDI_SYSEX);
			midiOut.sendMidiByte(0x47); // akai manufacturer code
			midiOut.sendMidiByte(0x00); // channel 0
			midiOut.sendMidiByte(0x42); // MULTI
			midiOut.sendMidiByte(0x48); // using an Akai S2000
			midiOut.sendMidiByte(0x00); // Part 1
			midiOut.sendMidiByte(0x00); // transpose
			midiOut.sendMidiByte(0x01); // Access Multi Parts
			midiOut.sendMidiByte(0x4B); // offset
			midiOut.sendMidiByte(0x00); // offset
			midiOut.sendMidiByte(0x01); // Field size = 1
			midiOut.sendMidiByte(0x00); // Field size = 1
			midiOut.sendMidiByte(0x04); // pitch value = 4
			midiOut.sendMidiByte(0x00); // offset
			midiOut.sendMidiByte(MIDI_SYSEX_END);
			
			// send again using a vector
			//
			// sends all bytes within one message
			//
			vector<unsigned char> sysexMsg;
			sysexMsg.push_back(MIDI_SYSEX);
			sysexMsg.push_back(0x47);
			sysexMsg.push_back(0x00);
			sysexMsg.push_back(0x42);
			sysexMsg.push_back(0x48);
			sysexMsg.push_back(0x00);
			sysexMsg.push_back(0x00);
			sysexMsg.push_back(0x01);
			sysexMsg.push_back(0x4B);
			sysexMsg.push_back(0x00);
			sysexMsg.push_back(0x01);
			sysexMsg.push_back(0x00);
			sysexMsg.push_back(0x04);
			sysexMsg.push_back(0x00);
			sysexMsg.push_back(MIDI_SYSEX_END);
			midiOut.sendMidiBytes(sysexMsg);
			
			// send again with the byte stream interface
			//
			// builds the message, then sends it on FinishMidi()
			//
			midiOut << StartMidi() << MIDI_SYSEX
					<< 0x47 << 0x00 << 0x42 << 0x48 << 0x00 << 0x00 << 0x01
					<< 0x4B << 0x00 << 0x01 << 0x00 << 0x04 << 0x00
					<< MIDI_SYSEX_END << FinishMidi();
			break;
		}
		
		// print the port list
		case '?':
			midiOut.listOutPorts();
			break;
		
		// note off using raw bytes
		case ' ':
			// send with the byte stream interface, noteoff for note 60
			midiOut << StartMidi() << 0x80 << 0x3C << 0x40 << FinishMidi();
			break;

		default:
    
			// send a note off if the key is a letter or a number
			if(isalnum(key)) {
				note = ofMap(key, 48, 122, 0, 127);
				velocity = 0;
				midiOut << NoteOff(channel, note, velocity); // stream interface
			}
			break;
	}


    recordingState = 0;
    if (myData.size() > 0) myKnn.train(myData);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	// x pos controls the pan (ctl = 10)
	pan = ofMap(x, 0, ofGetWidth(), 0, 127);
	midiOut.sendControlChange(channel, 10, pan);
	
	// y pos controls the pitch bend
	bend = ofMap(y, 0, ofGetHeight(), 0, MIDI_MAX_BEND);
	midiOut.sendPitchBend(channel, bend);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
