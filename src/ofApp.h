#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOsc.h"
#include "ofxRapidLib.h"
#include "MiMU.h"
#include "ParticleSystem.h"
#include "Metaball.hpp"

# define GLOVER 9000
# define GLOVE 8216

using namespace glm;

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void devUpdate();
		void performUpdate();
		
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

		int prevVelocity = 0;
		vec3 prevOrientation = vec3(0,0,0);
		quat prevQuaternion = quat(0,0,0,0);

		bool isRPI = true;


		//3D graphics
		ofEasyCam cam;
		ofLight light;
		ofMaterial material;
		ofMaterial myMeshMaterial;
		ofBoxPrimitive box;
		ofLight myLight1;
		ofLight myLight2;
		ofTexture mTex;


		float mySphereRadius;
		int mySetupMode; // 0 = plane, 1 = sphere, 2 = custom mesh
		ofMesh myInitialMesh;
		ParticleSystem myParticleSystem;

		//rapidmix
		std::vector<rapidLib::trainingExample> myData;
		int recordingState;
		rapidLib::classification myKnn;
		bool runToggle;
		int result;
		bool pageToggle;

		//metaballs
		    ofShader metaballShader;
    
		//needs to be const static to declare array
		const static int numBalls = 10;
		vector <Metaball> metaballs;

		//setunifrom3fv only accepts float *
		float data[numBalls*3];
		float amplitudes[numBalls];
		float frequencies[numBalls];

		float avgX, avgY, avgRadius;


};
