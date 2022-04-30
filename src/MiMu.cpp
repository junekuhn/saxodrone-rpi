#include "MiMU.h"

void MiMU::setup() {
    orientation = vec3(0.,0.,0.);
    accel = vec3(0., 0., 0.);
    gyro = vec3(0., 0., 0.);
    magnet = vec3(0., 0., 0.);
    gestureMode = false;
    pageToggle = false;
    gesture = 0;


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
                    // cout<< "changing to page " << page << endl;

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

}

void MiMU::draw() {
    
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