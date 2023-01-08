#pragma once
#include "Model.h"
#include "noise/noise1234.h"
//#include <main.cpp>

class Ditch {
public:

	// The plane size in vertices
	const int VER_LENGTH = 33;
	const int VER_WIDTH = 9;

	// The distance between vertices
	float verStepX = 0.0f;
	float verStepZ = 0.0f;

	// The plane size
	float planeLength = 0.0f;
	float planeWidth = 0.0f;

	float maxX = 0.0f;
	float minX = 0.0f;
	float maxZ = 0.0f;
	float minZ = 0.0f;

	glm::vec3 Position = glm::vec3(0.0, 0.0, 0.0f);//
	glm::vec3 Rotation = glm::vec3(0.0, 0.0, 0.0);// 
	glm::vec3 Scale = glm::vec3(0.50);//glm::vec3(noise1(runtme() + runtimeMS() * msScale));

	float displacementScaling = 15;
	float displacementVelocity = 0;
	int octaves = 8;

	Ditch() {

		// Loop through all vertices and check their in bounds
		for (int i = 0; i < theDitch.meshes[0].vertices.size(); i++) {
			if (theDitch.meshes[0].vertices[i].Position.x > maxX) {
				maxX = theDitch.meshes[0].vertices[i].Position.x;
			}
			else if (theDitch.meshes[0].vertices[i].Position.x < minX) {
				minX = theDitch.meshes[0].vertices[i].Position.x;
			}

			if (theDitch.meshes[0].vertices[i].Position.z > maxZ) {
				maxZ = theDitch.meshes[0].vertices[i].Position.z;
			}
			else if (theDitch.meshes[0].vertices[i].Position.z < minZ) {
				minZ = theDitch.meshes[0].vertices[i].Position.z;
			}
		}

		verStepZ = abs((maxZ-minZ)/VER_LENGTH);
		verStepX = abs((maxX - minX) / VER_WIDTH);

		if (vertexAt(2*verStepX, 5*verStepZ)) {
			std::cout << "APA";
		}


		//The individual seed of the meteor is set as the runtime
		displace(runtimeMS());
	}

	// Move every second vertex to the vertex previous to it, to create double vertices to make the displacement square
	/*void doubleVertices() {
		for (varannan vertex i xled) {
			for (varannan vertex i yled) {

			}
		}
	}*/

	// Create planes to build the ditch, add noise and move them to create the ditch. 
	/*void plane() {


	}*/

	//
	Vertex* vertexAt(float widthPos, float lengthPos) {
		for (int i = 0; i < theDitch.meshes[0].vertices.size(); i++) {
			if (theDitch.meshes[0].vertices[i].Position.x == widthPos) {
				if (theDitch.meshes[0].vertices[i].Position.z == lengthPos) {
					return &theDitch.meshes[0].vertices[i];
				}
			}
		}

		return nullptr;
	}
	

	//Generates the shapes of the ditch
	//Generates the random shape for the meteorite.
	void displace(float individualSeed) {
		//This is redundant for making the whole thing displaced.

		//Displace every vertex, and do it to follow the 1/f rule!!
		for (int i = 0; i < theDitch.meshes[0].vertices.size(); i++) {

			float displacement = 0.0f;

			//The model contains 3-4 vertices in each position, and a vertex is only part of one (?) triangle, therefore a vertex group needs to be modified.
			for (int k = 1; k < octaves + 1; k++) {
				displacement += noise(glm::vec2((theDitch.meshes[0].vertices[i].Position.x + 0.02) * individualSeed * pow(2, octaves),
												(theDitch.meshes[0].vertices[i].Position.z + 0.06) * individualSeed * pow(2, octaves))) / pow(2, octaves);

			}

			//displacement = theDitch.meshes[0].vertices[i].Position.x + theDitch.meshes[0].vertices[i].Position.z;

			theDitch.meshes[0].vertices[i].Position.y += displacement * displacementScaling;//The displacement is added in the direction of the normal 

		}
		
		// Calculate new normals
		


	}

	Model theDitch = Model("../assets/models/ditch/plane.obj");
	Shader ditchShader = Shader("../src/shaders/ditch.vs", "../src/shaders/ditch.fs");

private:



};