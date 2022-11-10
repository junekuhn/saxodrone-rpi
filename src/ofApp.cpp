#include "ofApp.h"
#include <math.h>

using namespace glm;
using namespace std;


//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(0);

	// print the available output ports to the console
	midiOut.listOutPorts();
    midiIn.listInPorts();

    // connect
	midiOut.openPort(1);
    midiIn.openPort(1);
    midiIn.ignoreTypes(false, false, false);
    // add ofApp as a listener
	midiIn.addListener(this);

	// print received messages to the console
	midiIn.setVerbose(true);

	//midiOut.openPort("IAC Driver Pure Data In"); // by name


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
    debugMode = false;

    ofSetFrameRate(30);

    mySphereRadius = 300;
    int paramGridSizeX = 40;
    int paramGridSizeY = 50;
    ofPrimitiveMode curDisplayMode = OF_PRIMITIVE_TRIANGLES;
    myParticleSystem.setupSphere(paramGridSizeX, paramGridSizeY, mySphereRadius, curDisplayMode);

    //3D graphics setup
    ofEnableNormalizedTexCoords();
    material.setDiffuseColor(ofColor(255, 0, 0));
    
    light.setPointLight();
    light.setDiffuseColor(ofColor(255));
    light.setPosition(500, 0, 300);

    	// Setup material to use on the particle mesh
	myMeshMaterial.setDiffuseColor(ofFloatColor(0.905, 0.82, 0.99)); //lilac
	myMeshMaterial.setSpecularColor(ofFloatColor(0.7, 0.7, 0.7));
	myMeshMaterial.setAmbientColor(ofFloatColor(0.1, 0.1, 0.2));
	myMeshMaterial.setShininess(50.0);

	// Setup lights (settings for discrete GPU)
	myLight1.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
	myLight1.setSpecularColor(myLight1.getDiffuseColor());
	myLight1.setPosition(vec3(120.0, 50.0, 120.0));

	myLight2.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
	myLight2.setSpecularColor(myLight2.getDiffuseColor());
	myLight2.setPosition(vec3(-120.0, 50.0, -120.0));
    
    box.setPosition(-50,50,0);
    box.set(200, 100, 100);

    // metaballShader.load("metaball");
    
    // for(int i = 0; i<numBalls; i++) {
    //     Metaball metaball;
    //     metaballs.push_back(metaball);
    // }

    // ofDisableArbTex();

}

//--------------------------------------------------------------
void ofApp::update(){
    
    rightGlove.update();

    // Update the particles
    //amplitude, frequency, scale
	myParticleSystem.update(8.0, 1.0, 3.0);

    if(debugMode) {
        devUpdate();
    } else {
        performUpdate();
    }

    // for( int i = 0; i< numBalls; i++) {
    //     metaballs[i].update();
    // }

}

//exposes all midi parameters for testing
void ofApp::devUpdate() {
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
                velocity  = ofMap(rightGlove.pitch, -M_PI/2, M_PI/2, 127, -127);

                if(velocity<0) {
                    velocity = 0;
                }
            } else {
                velocity = rightGlove.pitch;
            }

            velocity = rightGlove.ease(prevVelocity, velocity);
            prevVelocity = velocity;

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

//minimal parameters for performance
void ofApp::performUpdate() {

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

            velocity = rightGlove.ease(prevVelocity, velocity);
            prevVelocity = velocity;

            cout<<"velocity is " << velocity<<endl;
                        //slide roll to be at the bottom

            switch (gestureSwitch) {
                //finger point
                case 0:
                    //mix
                    cc = 18;
                    break;
                //fist
                case 1:
                    //preset selection
                    cc = 127;
                    break;
                // open hand - default
                case 2:
                    //exp
                    cc = 4;
                    break;
                // OK hand
                case 3:
                    //delay feedback
                    cc = 27;
                    break;
                // climber
                case 4:
                    // delay level
                    cc = 24;
                    break;
                // puppet hand
                case 5:
                    //portamento
                    cc = 22;
                    break;
                default:
                    break;

            }
            // cout<<cc<<endl;
            cout<<"velocity"<<velocity<<endl;
            if(cc == 127) {
                    // code that changes preset based on direction  
                    // accesses the first 6 presets
                    if(rightGlove.direction != currentPgm) midiOut.sendProgramChange(channel, rightGlove.direction);
                    currentPgm = rightGlove.direction;
                    cout<< "program change " << rightGlove.direction << endl;
            } else {
                    midiOut.sendControlChange(channel, cc, velocity);
            }
    } 
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofSetColor(255);

    //   ofDisableArbTex();

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
      //do all our 3D drawing here
    
    ofEnableLighting();
    
    light.enable();
    
    // material.begin();

    ofPushMatrix();
    if(rightGlove.usingGlover) {
        vec3 eased;
        eased.x = rightGlove.ease(prevOrientation.x, rightGlove.orientation.x);
        eased.y = rightGlove.ease(prevOrientation.y, rightGlove.orientation.y);
        eased.z = rightGlove.ease(prevOrientation.z, rightGlove.orientation.z);
        //you run into gimbal lock pretty often
        ofRotateXRad(eased.x);
        ofRotateYRad(eased.y);
        ofRotateZRad(eased.z);
        prevOrientation = eased;
    } else {
        quat easedQ;
        easedQ.x = rightGlove.ease(prevQuaternion.x, rightGlove.quaternion.x);
        easedQ.y = rightGlove.ease(prevQuaternion.y, rightGlove.quaternion.y);
        easedQ.z = rightGlove.ease(prevQuaternion.z, rightGlove.quaternion.z);
        easedQ.w = rightGlove.ease(prevQuaternion.w, rightGlove.quaternion.w);

        //for now this works but its broken
        // ofRotateRad(easedQ.w*2, easedQ.x, easedQ.y, easedQ.z);
        ofQuaternion of_quat{ easedQ };
        ofMatrix4x4 rotation_matrix = ofMatrix4x4::newRotationMatrix( easedQ );
	    ofMultMatrix(rotation_matrix);
        //rotate quaternion
        prevQuaternion = easedQ;
    }

    //     float sumX = 0, sumY = 0, radii = 0;
   
    // //set the data in groups of three, for each parameter
    // for(int i = 0; i<numBalls; i++) {
        
    //     //feature extraction for shader
    //     data[i*3] = metaballs[i].pos.x;
    //     data[i*3+1] = metaballs[i].pos.y;
    //     data[i*3+2] = metaballs[i].radius;
        
        
    // }
    
    // avgX = sumX/numBalls;
    // avgY = sumY/numBalls;
    // avgRadius = radii/numBalls;
    
    // metaballShader.begin();
    
    // metaballShader.setUniform3fv("metaballs", data, numBalls);
    // metaballShader.setUniform1i("num_balls", numBalls);
    // metaballShader.setUniform1f("HEIGHT", ofGetHeight());
    // metaballShader.setUniform1f("WIDTH", ofGetWidth());
    
    // ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());
    myMeshMaterial.begin();
    myParticleSystem.draw();

    // metaballShader.end();
	// Draw the particles
	myMeshMaterial.end();

    ofPopMatrix();

    //disable everything
    // material.end();
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
        // g
        case 103:
            rightGlove.toggleGlover();
            rightGlove.setup();
            break;
        // d
        case 100:
            debugMode = !debugMode;
            break;
        //f for forwards
        case 102:
            rightGlove.setForwards();
            break;
        //b for button (gesturemode)
        case 98:
            rightGlove.toggleGestureMode();
            break;

		case '?':
			midiOut.listOutPorts();
			break;
		default:
			break;
	}


    recordingState = 0;
    if (myData.size() > 0) myKnn.train(myData);
}

void ofApp::newMidiMessage(ofxMidiMessage& msg) {

	// add the latest message to the message queue
	midiMessages.push_back(msg);

	// remove any old messages if we have too many
	while(midiMessages.size() > maxMessages) {
		midiMessages.erase(midiMessages.begin());
	}
}

void ofApp::exit() {

	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

void writeSysexFile(std::vector<unsigned char> &bytes) {
    string filename = "test";
    string filePath = ofToDataPath(filename);

        // If the data directory doesn't already exist, create it
    if (!ofDirectory::doesDirectoryExist("")) {
        ofDirectory::createDirectory("");
    }

    ofstream outfile(filePath);

    if (!outfile.is_open()) {
        cout << "Error: couldn't open file " << filename << " for writing\n";
        return;
    }

    for(int i = 0; i<bytes.size(); i++) {
        outfile << bytes[i];
    }

    outfile.close();

}
