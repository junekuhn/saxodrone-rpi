#pragma once

#include "ofMain.h"
#include "Particle.h"

using namespace glm;

class ParticleSystem {
public:
	void setupPlane(int gridSizeX, int gridSizeY, float planeRangeX, float planeRangeY, ofPrimitiveMode displayMode);
	void setupSphere(int gridSizeX, int gridSizeY, float sphereRadius, ofPrimitiveMode displayMode);
	void setupUsingMesh(ofMesh inputMesh, ofPrimitiveMode displayMode);
	void update(float amplitude, float frequency, float scale);
	void draw();
	void setOrientation(vec3 rotation);
	void setOrientation(vec4 rotation);
	ofMesh getMesh();

private:
	int getParticleIndex(int x, int y);

	vector<Particle> myParticles;
	ofMesh myMesh;
	ofPrimitiveMode myDisplayMode;
	int myGridSizeX;
	int myGridSizeY;
};
