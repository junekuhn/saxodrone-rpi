#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOSC.h"
#include "ofxRapidLib.h"

# define GLOVER 9000
# define GLOVE 8216

using namespace glm;

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

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


		//output features
		ofxMidiOut midiOut;
		int channel;
		unsigned int currentPgm;
		int note, velocity;
		int pan, bend, touch, polytouch;
		float roll = 0;
		float pitch = 0;
		int expr, tempo, mix;
		int page;

		bool isRPI = false;
		bool usingGlover = false;

		//3D graphics
		ofEasyCam cam;
		ofLight light;
		ofMaterial material;
		ofBoxPrimitive box;

		//rapidmix
		std::vector<rapidLib::trainingExample> myData;
		int recordingState;
		rapidLib::classification myKnn;
		bool runToggle;
		int result;
		bool pageToggle;


};
