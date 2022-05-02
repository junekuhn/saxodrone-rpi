#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOsc.h"
#include "ofxRapidLib.h"
#include "MiMU.h"

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
		
		MiMU rightGlove;
		bool debugMode;

		//output features
		ofxMidiOut midiOut;
		int channel;
		unsigned int currentPgm;
		int note, velocity;
		int pan, bend, touch, polytouch;
		float roll = 0;
		float pitch = 0;
		float yaw = 0;
		int expr, tempo, mix;
		int page;

		bool isRPI = true;


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
