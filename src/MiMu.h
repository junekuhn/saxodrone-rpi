#pragma once

#include "ofMain.h"
#include "ofxOSC.h"
#include "ofxRapidLib.h"

# define GLOVER 9000
# define GLOVE 8216

class MiMu {
    public:

        void setup();
		void update();
		void draw();
        
        ofxOscReceiver rx;
		string gestureLookup(int gesture);

		//incoming features
		vec3 orientation;
		vec3 gyro;
		vec3 accel;
		vec3 magnet;
		float baro;
		bool buttonState;

		bool gestureMode;
		int gesture;
		quat quaternion;
		double bends[8];

    		//rapidmix
		std::vector<rapidLib::trainingExample> myData;
		int recordingState;
		rapidLib::classification myKnn;
		bool runToggle;
		int result;
		bool pageToggle;

    private:

}