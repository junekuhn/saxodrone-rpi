#include "MiMU.h"

void MiMU::setup() {
    orientation = vec3(0.,0.,0.);
    accel = vec3(0., 0., 0.);
    gyro = vec3(0., 0., 0.);
    magnet = vec3(0., 0., 0.);
    forwardQuaternion = quat(0,0,0,0);
    forwardOrientation = vec3(0,0,0);
    gestureMode = false;
    pageToggle = false;
    gesture = -1;
    direction = -1;
    easingToggle = true;
    easingAmount = 0.1;


    if(usingGlover) {
        rx.setup(GLOVER);
    } else {
        rx.setup(GLOVE);
    }
}

void MiMU::update() {

      // check for waiting messages
	while(rx.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		rx.getNextMessage(m);

        if(usingGlover) {
            // switch for incoming Glover messages
            if(m.getAddress() == "/roll"){
                // should be from 0 to 2pi
                orientation.x = m.getArgAsFloat(0);
            }
            // check for mouse button message
            // x is roll
            // y is pitch
            // z is yaw
            else if(m.getAddress() == "/pitch"){
                //should be coming in from 0 to 3.14
                orientation.z = m.getArgAsFloat(0);
            } else if (m.getAddress() == "/yaw") {
                // should be coming in at 0 to 6.28
                orientation.y = m.getArgAsFloat(0);    
            }
             else if (m.getAddress() == "/button") {
                gestureMode = m.getArgAsBool(0);  
                // cout<<gestureMode<<endl; 
            } else if (m.getAddress() == "/gesture") {
                gesture = m.getArgAsInt(0);
            } else if (m.getAddress() =="/direction") {
                direction = m.getArgAsInt(0);
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
                    // cout<< "changing to page " << page << endl;

                } else {
                    //this means the button is held down
                     pageToggle = true;
                }
            } 
            else if (m.getAddress() == "/quaternion") {
                quaternion.x = m.getArgAsFloat(0);
                quaternion.y = m.getArgAsFloat(1);
                quaternion.z = m.getArgAsFloat(2);
                quaternion.w = m.getArgAsFloat(3);
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
        pitch = orientation.z;
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

}

void MiMU::draw(int result) {

    int x = 60;

    ofDrawBitmapString("Saxodrone + MiMU Glove", ofGetWidth()/2-100, 40);

    if(gestureMode) {
        ofDrawBitmapString("Gesture Mode Enabled", x, 50);
    } else {
        ofDrawBitmapString("Gesture Mode Disabled", x, 50);
    }

    if(usingGlover) {
        ofDrawBitmapString("Using Glover", x, 90);
        ofDrawBitmapString(gestureLookup(gesture), x, 30);
        ofDrawBitmapString(directionLookup(direction) , x, 70);
    } else {
        ofDrawBitmapString("No Glovers Allowed", x, 90);
        ofDrawBitmapString(gestureLookup(result-1), x, 30);
        ofDrawBitmapString(directionLookup(direction), x, 70);
    }
}

string MiMU::gestureLookup(int gesture) {

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

string MiMU::directionLookup(int direction) {

    string result;
    switch(direction) {
        case 0:
            result = "Forwards";
            break;
        case 1:
            result = "Backwards";
            break;
        case 2:
            result = "Left";
            break;
        case 3:
            result = "Right";
            break;
        case 4:
            result = "Up";
            break;
        case 5:
            result = "Down";
            break;
        default:
            result = "No direction detected";
            break;
    }

    return result;
}

float MiMU::ease(float previous, float current) {
    float diff = current - previous;
    float output = previous + easingAmount * diff;
    return output;
}

int MiMU::ease(int previous, int current) {
    int diff = current - previous;
    int output = previous + easingAmount * diff;
    return output;
}

void MiMU::toggleGlover() {
    usingGlover = !usingGlover;
}

void MiMU::setForwards() {
    forwardQuaternion = quaternion;
    forwardOrientation = orientation;
}