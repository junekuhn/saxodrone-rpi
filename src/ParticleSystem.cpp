#include "ParticleSystem.h"
#include "helpers.h"

//--------------------------------------------------------------
void ParticleSystem::setupPlane(int gridSizeX, int gridSizeY, float planeRangeX, float planeRangeY, ofPrimitiveMode displayMode) {
	// Clear any existing particle and mesh data
	myParticles.clear();
	myMesh.clear();

	// Store grid size values into member variables
	myGridSizeX = gridSizeX;
	myGridSizeY = gridSizeY;

	// Set the display mode
	myDisplayMode = displayMode;
	myMesh.setMode(myDisplayMode);

	// Initialize the particles
	for (int i = 0; i < myGridSizeX; i++)
		for (int j = 0; j < myGridSizeY; j++) {
			vec3 pos = vec3(
				ofMap(i, 0, myGridSizeX - 1, -0.5 * planeRangeX, 0.5 * planeRangeX),
				0.0,
				ofMap(j, 0, myGridSizeY - 1, -0.5 * planeRangeY, 0.5 * planeRangeY));
			vec3 dir = vec3(0, 1, 0);
			Particle par;
			par.setup(pos, dir);
			myParticles.push_back(par);
		}

	// Create a vertex for each particle
	for (int i = 0; i < myParticles.size(); i++) {
		vec3 pos = myParticles[i].getPos();
		myMesh.addVertex(pos);
	}

	// Initialize the mesh
	if (myDisplayMode == OF_PRIMITIVE_POINTS) {
		// For points we don't need to declare anything more
	}
	else if (myDisplayMode == OF_PRIMITIVE_LINES) {
		// Declare lines connecting the vertices in a square grid using the vertex indices
		// We add lines by creating a list of pairs of vertex indices that should be
		// connected by lines
		for (int i = 0; i < myGridSizeX; i++) {
			for (int j = 0; j < myGridSizeY; j++) {
				if (i < myGridSizeX - 1) {
					myMesh.addIndex(getParticleIndex(i, j));
					myMesh.addIndex(getParticleIndex(i + 1, j));
				}

				if (j < myGridSizeY - 1) {
					myMesh.addIndex(getParticleIndex(i, j));
					myMesh.addIndex(getParticleIndex(i, j + 1));
				}
			}
		}
	}
	else if (myDisplayMode == OF_PRIMITIVE_TRIANGLES) {
		// Declare two triangles for each square in the grid
		for (int i = 0; i < myGridSizeX - 1; i++) {
			for (int j = 0; j < myGridSizeY - 1; j++) {
				myMesh.addTriangle(
					getParticleIndex(i, j),
					getParticleIndex(i + 1, j + 1),
					getParticleIndex(i + 1, j));

				myMesh.addTriangle(
					getParticleIndex(i, j),
					getParticleIndex(i, j + 1),
					getParticleIndex(i + 1, j + 1));
			}
		}

		// Add a normal for each vertex. This can be done by calling the
		// calcNormals helper function
		calcNormals(myMesh);
	}
	else {
		ofLogError("ParticleSystem::setup, displayMode set to invalid value");
	}
}

//--------------------------------------------------------------
void ParticleSystem::setupSphere(int gridSizeX, int gridSizeY, float sphereRadius, ofPrimitiveMode displayMode) {
	// Setting up particles into a initial spherical formation

	// Clear any existing particle and mesh data
	myParticles.clear();
	myMesh.clear();

	// Store grid size values into member variables
	myGridSizeX = gridSizeX;
	myGridSizeY = gridSizeY;

	// Set the display mode
	myDisplayMode = displayMode;
	myMesh.setMode(myDisplayMode);

	// Initialize the particles
	for (int i = 0; i < myGridSizeX; i++)
		for (int j = 0; j < myGridSizeY; j++) {
			// Use spherical co-ordinates to calculate initial position
			float theta = ofDegToRad(ofMap(i, 0, myGridSizeX, 360.0, 0.0));
			float phi = ofDegToRad(ofMap(j, -1, myGridSizeY, 0.0, 180.0));
			vec3 pos = sphereRadius * vec3(sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta));
			vec3 dir = normalize(pos);
			Particle par;
			par.setup(pos, dir);
			myParticles.push_back(par);
		}

	// Create additional particles for the top and bottom
	Particle topParticle;
	topParticle.setup(vec3(0, sphereRadius, 0), vec3(0, 1, 0));
	int topParticleIndex = myParticles.size();
	myParticles.push_back(topParticle);

	Particle bottomParticle;
	bottomParticle.setup(vec3(0, -sphereRadius, 0), vec3(0, -1, 0));
	int bottomParticleIndex = myParticles.size();
	myParticles.push_back(bottomParticle);

	// Create a vertex for each particle
	for (int i = 0; i < myParticles.size(); i++) {
		vec3 pos = myParticles[i].getPos();
		myMesh.addVertex(pos);
		// ofVec2f texCoords = {((float)(i) / widthTexture), ((float)(j) / heightTexture)};
	}

	// Initialize the mesh
	if (myDisplayMode == OF_PRIMITIVE_POINTS) {
		// For points we don't need to declare anything more
	}
	else if (myDisplayMode == OF_PRIMITIVE_LINES) {
		// Declare lines connecting the vertices in a square grid using the vertex indices
		// We add lines by creating a list of pairs of vertex indices that should be
		// connected by lines
		for (int i = 0; i < myGridSizeX; i++) {
			for (int j = 0; j < myGridSizeY; j++) {

				myMesh.addIndex(getParticleIndex(i, j));
				myMesh.addIndex(getParticleIndex(i + 1, j));

				if (j < myGridSizeY - 1) {
					myMesh.addIndex(getParticleIndex(i, j));
					myMesh.addIndex(getParticleIndex(i, j + 1));
				}
			}
		}

		// Connect the bottom and top verices to all the other vertices in the first and last rows
		for (int i = 0; i < myGridSizeX; i++) {
			myMesh.addIndex(topParticleIndex);
			myMesh.addIndex(getParticleIndex(i, 0));

			myMesh.addIndex(bottomParticleIndex);
			myMesh.addIndex(getParticleIndex(i, myGridSizeY - 1));
		}
	}
	else if (myDisplayMode == OF_PRIMITIVE_TRIANGLES) {
		// Declare two triangles for each square in the grid
		for (int i = 0; i < myGridSizeX; i++) {
			for (int j = 0; j < myGridSizeY - 1; j++) {
				myMesh.addTriangle(
					getParticleIndex(i, j),
					getParticleIndex(i + 1, j + 1),
					getParticleIndex(i + 1, j));

				myMesh.addTriangle(
					getParticleIndex(i, j),
					getParticleIndex(i, j + 1),
					getParticleIndex(i + 1, j + 1));
			}
		}

		// Connect the bottom and top verices to all the other vertices in the first and last rows
		for (int i = 0; i < myGridSizeX; i++) {
			myMesh.addTriangle(
				topParticleIndex,
				getParticleIndex(i + 1, 0),
				getParticleIndex(i, 0));

			myMesh.addTriangle(
				bottomParticleIndex,
				getParticleIndex(i, myGridSizeY - 1),
				getParticleIndex(i + 1, myGridSizeY - 1));
		}

		// Add a normal for each vertex. This can be done by calling the
		// calcNormals helper function
		calcNormals(myMesh);
	}
	else {
		ofLogError("ParticleSystem::setup, displayMode set to invalid value");
	}
}

//--------------------------------------------------------------
void ParticleSystem::setupUsingMesh(ofMesh inputMesh, ofPrimitiveMode displayMode) {
	// Clear any existing particles and mesh data
	myParticles.clear();

	// Copy the input mesh into myMesh
	myMesh = inputMesh;

	// Set the display mode
	myDisplayMode = displayMode;
	myMesh.setMode(myDisplayMode);

	// If the mesh doesn't have any normals, calculate them
	if (myMesh.getNumNormals() != myMesh.getNumVertices()) {
		calcNormals(myMesh);
	}

	// Declare a particle for each vertex, using the normals
	// from the mesh as the direction to move each particle
	for (int i = 0; i < myMesh.getNumVertices(); i++) {
		vec3 p = myMesh.getVertex(i);
		vec3 n = myMesh.getNormal(i);
		Particle par;
		par.setup(p, n);
		myParticles.push_back(par);
	}

	// Assuming that the original .ply file was a set of triangles we've
	// already got the triangle list data.

	// If the display mode is lines we need to convert from the triangles
	// to lines

	if (myDisplayMode == OF_PRIMITIVE_LINES) {
		// Calculated index list for lines. For simplicity here we're just
		// going to declare every edge of every triangle as a line. This
		// means that a lot of lines are probably going to get drawn twice.
		// This could be cleaned up, but would require quite a lot more
		// processing for the general case to work with any mesh.

		vector<ofIndexType> lineIndexList;
		int numTriangles = myMesh.getNumIndices() / 3;
		for (int i = 0; i < numTriangles; i++) {
			// Declare three lines for each triangle
			ofIndexType v0 = myMesh.getIndex(3 * i);
			ofIndexType v1 = myMesh.getIndex(3 * i + 1);
			ofIndexType v2 = myMesh.getIndex(3 * i + 2);

			lineIndexList.push_back(v0);
			lineIndexList.push_back(v1);

			lineIndexList.push_back(v1);
			lineIndexList.push_back(v2);

			lineIndexList.push_back(v2);
			lineIndexList.push_back(v0);
		}

		// Replace the indices in the mesh with lineIndexList
		myMesh.clearIndices();
		myMesh.addIndices(lineIndexList);
	}

	// Set the correct display mode on the mesh
	myMesh.setMode(myDisplayMode);
}

//--------------------------------------------------------------
void ParticleSystem::update(float amplitude, float frequency, float scale) {
	for (int i = 0; i < myParticles.size(); i++) {
		myParticles[i].update(amplitude, frequency, scale);
		myMesh.setVertex(i, myParticles[i].getPos());
	}

	// If we've got a mesh of triangles we need to update the vertex normals
	if (myDisplayMode == OF_PRIMITIVE_TRIANGLES) {
		calcNormals(myMesh);
	}
}

//--------------------------------------------------------------
void ParticleSystem::draw() {
	myMesh.draw();
}

//--------------------------------------------------------------
int ParticleSystem::getParticleIndex(int x, int y) {
	x = ofWrap(x, 0, myGridSizeX);
	y = ofWrap(y, 0, myGridSizeY);
	return myGridSizeY * x + y;
}

//--------------------------------------------------------------
ofMesh ParticleSystem::getMesh() {
	return myMesh;
}

// void ParticleSystem::setOrientation(vec3 rotation) {
// 	myMesh.setOrientation(rotation);
// }

// void ParticleSystem::setOrientation(vec4 rotation) {
// 	myMesh.setOrientation(rotation);
// }