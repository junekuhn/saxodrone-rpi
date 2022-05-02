#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxRapidLib.h"

# define GLOVER 9000
# define GLOVE 8216

using namespace glm;

class MiMU {
    public:

        void setup();
		void update();
		void draw(int result);

		bool usingGlover = true;

        ofxOscReceiver rx;
		string gestureLookup(int gesture);
		string directionLookup(int direction);

		//incoming features
		vec3 orientation;
		vec3 gyro;
		vec3 accel;
		vec3 magnet;
		float baro;
		bool buttonState;

		bool gestureMode;
		int gesture;
		int direction;
		quat quaternion;
		double bends[8];

		float roll = 0;
		float pitch = 0;
		float yaw = 0;

    		//rapidmix
		std::vector<rapidLib::trainingExample> myData;
		int recordingState;
		rapidLib::classification myKnn;
		bool runToggle;
		int result;
		bool pageToggle;

    private:

};
