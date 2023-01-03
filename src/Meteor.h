#pragma once
#include "Model.h"
#include "noise/noise1234.h"

class Meteor
{
public:

	float msScale = 0.1;
	float noise1Scale = 5;
	float MakeItSmaller = 0.3f;

	Meteor() {
	//Generate random shape
	//Generate random position and rotation speed
	//Calculate the correct normals (so that the displacement is correct).
		for (int i = 0; i < theMeteor.meshes[0].vertices.size(); i++) {
			//Calculating the direction from the midpoint of the meteor to the vertex
			theMeteor.meshes[0].vertices[i].Normal = glm::vec3(theMeteor.meshes[0].vertices[i].Position - Position);

		}

		//The individual seed of the meteor is set as the runtime
		displace(runtimeMS());

	};
	glm::vec3 Position = glm::vec3(0.0, 0.0, 0.0f);//
	glm::vec3 Rotation = glm::vec3(0.0, 0.0, 0.0);// 
	glm::vec3 Scale = glm::vec3(0.50);//glm::vec3(noise1(runtime() + runtimeMS() * msScale));
	

	float displacementVelocity = 0;
	int octaves = 8;

	float displacementScaling = 10;

	//Generates the random shape for the meteorite.
	void displace(float individualSeed) {
		//This is redundant for making the whole thing displaced.
		Position = glm::vec3(noise1(runtime() + runtimeMS() * msScale) * noise1Scale - cos(runtimeMS()), noise1((runtime() + runtimeMS() * msScale) + 44) * noise1Scale, -10.0f); //random position
		Rotation = glm::vec3(noise1(runtime() + runtimeMS() * msScale) * noise1Scale * noise1Scale, noise1(runtime() + runtimeMS() * msScale), noise1(runtime() + runtimeMS() * msScale) * noise1Scale); //Rotation around its own axis
		Scale = glm::vec3(noise1(runtime() + runtimeMS() * msScale + 2.7)*noise1Scale*MakeItSmaller);
	
		glm::vec3 tempPos = theMeteor.meshes[0].vertices[0].Position;


		//Displace every vertex, and do it to follow the 1/f rule!!
		for (int i = 0; i < theMeteor.meshes[0].vertices.size(); i++) {
			
			float displacement = 0.0f;
	
		
			//Choose all vertexes in the same spot, Aka, find all other vertices in the same spot.
			for (int n = 0; n < theMeteor.meshes[0].vertices.size(); n++) {

			}


			//The model contains 3-4 vertices in each position, and a vertex is only part of one (?) triangle, therefore a vertex group needs to be modified.
			for (int k = 1; k < octaves + 1; k++) {
				displacement += gradientNoise(glm::vec3(theMeteor.meshes[0].vertices[i].Position.x * individualSeed * pow(2, octaves),
														theMeteor.meshes[0].vertices[i].Position.y * individualSeed * pow(2, octaves),
														theMeteor.meshes[0].vertices[i].Position.z * individualSeed * pow(2, octaves))) / pow(2, octaves);

				
			}

			theMeteor.meshes[0].vertices[i].Position += displacement * normalize(theMeteor.meshes[0].vertices[i].Normal) * displacementScaling;//The displacement is added in the direction of the normal 
	
		}

	}

	//Model theMeteor = Model("../assets/models/meteoritebase/meteoritebase.obj");
	Model theMeteor = Model("../assets/models/lowpolymeteor/meteor.obj");
	Shader meteorShader = Shader("../src/shaders/meteorite.vs", "../src/shaders/meteorite.fs");

private:
	


};
