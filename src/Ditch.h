#pragma once
#include "Model.h"
#include "noise/noise1234.h"
//#include <main.cpp>

class Ditch {
public:

	float maxHeight = 0.1;
	float minHeight = -0.1;

	glm::vec3 Position = glm::vec3(0.0, 0.0, 0.0f);//
	glm::vec3 Rotation = glm::vec3(0.0, 0.0, 0.0);// 
	glm::vec3 Scale = glm::vec3(0.50);//glm::vec3(noise1(runtme() + runtimeMS() * msScale));

	float displacementScaling = 15;
	float displacementVelocity = 0;
	int octaves = 8;

	Ditch() {

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
	/*
		void plane(){


		}
	*/

	//Generates the shapes of the ditch
	//Generates the random shape for the meteorite.
	void displace(float individualSeed) {
		//This is redundant for making the whole thing displaced.


		//Displace every vertex, and do it to follow the 1/f rule!!
		for (int i = 0; i < theDitch.meshes[0].vertices.size(); i++) {

			float displacement = 0.0f;
			glm::vec3 displacementDirection = glm::vec3(theDitch.meshes[0].vertices[i].Position - Position);

			//The model contains 3-4 vertices in each position, and a vertex is only part of one (?) triangle, therefore a vertex group needs to be modified.
			for (int k = 1; k < octaves + 1; k++) {
				displacement += gradientNoise(glm::vec3(theDitch.meshes[0].vertices[i].Position.x * individualSeed * pow(2, octaves),
					theDitch.meshes[0].vertices[i].Position.y * individualSeed * pow(2, octaves),
					theDitch.meshes[0].vertices[i].Position.z * individualSeed * pow(2, octaves))) / pow(2, octaves);

			}

			//Calculate the new normal
			/*glm::mat4 rot = glm::mat4(1.0f);

			glm::vec3 Rotation = glm::vec3(acos(displacement),asin(displacement),);

			rot = glm::rotate(rot, Rotation.x, glm::vec3(1, 0, 0));
			rot = glm::rotate(rot, Rotation.y, glm::vec3(0, 1, 0));
			rot = glm::rotate(rot, Rotation.z, glm::vec3(0, 0, 1));
			*/
			theDitch.meshes[0].vertices[i].Position += displacement * normalize(displacementDirection) * displacementScaling;//The displacement is added in the direction of the normal 

		}

	}

	Model theDitch = Model("../assets/models/ditch/ditch.obj");
	Shader ditchShader = Shader("../src/shaders/ditch.vs", "../src/shaders/ditch.fs");

private:



};