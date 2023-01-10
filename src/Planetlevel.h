#pragma once
#include "Level.h"
#include "Meteor.h"
#include "Plane.h"
#include <vector>
#include <cmath>
#include <iostream>

class Planetlevel : public Level
{
public:
	std::vector<Plane> plane;
	std::vector<Meteor> meteors;
	int numberOfMeteorsLimit = 1;
	int milliSecsBetweenSpawns = 800;
	int spawnCluster = 1;
	int prevSpawnTime = 0;
	int numberOfMet = 0;
	double time = 0.0;
	double intPart = 0.0;
	
	bool fog = true;
	
	glm::vec4 bgColor = glm::vec4(0.8, 0.67, 0.54, 1.0f);

	glm::vec3 lightDirection = glm::normalize(glm::vec3(0, -1, 0));
	glm::vec3 lightColor = glm::vec3(1.0f, 0.5f, 0.0f);
	float ambientScaling = 1.0;


	float getAmbientScaling() override {
		return ambientScaling;
	}


	glm::vec3 getLevelColor() override {
		
		return lightColor;
	}

	glm::vec3 getLightDirection() override {
		return lightDirection;
	}

	void generate() override {

		//Generate the lavaplane
		if (plane.size() == 0) {

			//std::cout << "Lightcolor: (" << lightColor.x << ", " << lightColor.y << ", " << lightColor.z << ")" << std::endl;
			//Plane newPlane = Plane();
			Plane newPlane = Plane("../src/shaders/lava.vs", "../src/shaders/lava.fs");

			plane.push_back(newPlane);

			for (int i = 0; i < newPlane.thePlane.meshes.size(); i++) {
				meshes.push_back(newPlane.thePlane.meshes[i]); //Adding the meshes of each meteor in to the level so that we can check for collision
			}
		}
		//Generate the meteors


		//std::cout << "Number of meteors: " << numberOfMeteorsLimit << std::endl;
		if (meteors.size() < numberOfMeteorsLimit && prevSpawnTime != runtimeMS()) {
			for (int k = 0; k < spawnCluster; k++) {
				Meteor newMet;
				newMet.Position = glm::vec3(noise1(runtime() + runtimeMS() * 0.1) * 5 - cos(runtimeMS()), (rand() % 3 + 3) * 1.1, -(rand() % 10) * 1.1); //random position;
				//glm::mat4 model = newMet.theMeteor.update(newMet.Position, newMet.Rotation, newMet.Scale);
				prevSpawnTime = runtimeMS();

				meteors.push_back(newMet);


				for (int i = 0; i < newMet.theMeteor.meshes.size(); i++) {
					meshes.push_back(newMet.theMeteor.meshes[i]); //Adding the meshes of each meteor in to the level so that we can check for collision
				}

			}
			if(numberOfMeteorsLimit < 1000 && prevSpawnTime % 9 == 0) {
				numberOfMeteorsLimit++;
			}
		}

		//Generate the walls
	}

	void draw(glm::mat4 projection, glm::mat4 view, glm::vec4 metColor, float dT) override {
		//Calculate the matrices for the plane
		for (int i = 0; i < plane.size(); i++) {
			plane[i].planeShader.use();
			plane[i].planeShader.setFloat("deltaTime", dT);
			plane[i].planeShader.setBool("bFog", fog);

			glm::mat4 model = glm::mat4(1.0f);
			
			model = glm::translate(model, plane[i].Position);

			model = glm::rotate(model, plane[i].Rotation.x, glm::vec3(1, 0, 0));
			model = glm::rotate(model, plane[i].Rotation.y, glm::vec3(0, 1, 0));
			model = glm::rotate(model, plane[i].Rotation.z, glm::vec3(0, 0, 1));
			model = glm::scale(model, plane[i].Scale);

			glm::mat4 rotation = glm::mat4(1.0f);
			rotation = glm::rotate(rotation, plane[i].Rotation.x, glm::vec3(1, 0, 0));
			rotation = glm::rotate(rotation, plane[i].Rotation.y, glm::vec3(0, 1, 0));
			rotation = glm::rotate(rotation, plane[i].Rotation.z, glm::vec3(0, 0, 1));

			plane[i].planeShader.setVec3("meteorPosition", plane[i].Position);
			plane[i].planeShader.setVec3("shipDirection", ship.shipDirection());
			plane[i].planeShader.setVec3("lightDirection", lightDirection);

			plane[i].planeShader.setVec3("shipPosition", glm::vec3(ship.spacePosition, 0.0));

			plane[i].planeShader.setMat4("rotation", rotation);
			plane[i].planeShader.setMat4("model", model);
			plane[i].planeShader.setMat4("projection", projection);
			plane[i].planeShader.setMat4("view", view);

			plane[i].thePlane.Draw(plane[i].planeShader);
		}
		//Calculate the matrices for the meteors
		for (int i = 0; i < meteors.size(); i++) {
			meteors[i].meteorShader.use();

			meteors[i].meteorShader.setVec4("metColor", metColor);
			meteors[i].meteorShader.setVec3("lightColor", lightColor);
			meteors[i].meteorShader.setBool("bFog", fog);
			//glm::mat4 model = meteors[i].theMeteor.update(meteors[i].Position, meteors[i].Position, meteors[i].Scale);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, meteors[i].Position);
			//std::cout << meteors[i].Position.z << "\n";
			model = glm::rotate(model, meteors[i].Rotation.x, glm::vec3(1, 0, 0));
			model = glm::rotate(model, meteors[i].Rotation.y, glm::vec3(0, 1, 0));
			model = glm::rotate(model, meteors[i].Rotation.z, glm::vec3(0, 0, 1));
			model = glm::scale(model, meteors[i].Scale);

			glm::mat4 rotation = glm::mat4(1.0f);
			rotation = glm::rotate(rotation, meteors[i].Rotation.x, glm::vec3(1, 0, 0));
			rotation = glm::rotate(rotation, meteors[i].Rotation.y, glm::vec3(0, 1, 0));
			rotation = glm::rotate(rotation, meteors[i].Rotation.z, glm::vec3(0, 0, 1));


			/*	for (int k = 0; k < meteors[i].theMeteor.meshes[0].vertices.size(); k++) {
					meteors[i].meteorShader.setVec3("normal", meteors[i].theMeteor.meshes[0].vertices[k].Normal;)
				}*/

			meteors[i].meteorShader.setVec3("meteorPosition", meteors[i].Position);
			meteors[i].meteorShader.setVec3("shipDirection", ship.shipDirection());
			meteors[i].meteorShader.setVec3("lightDirection", lightDirection);

			meteors[i].meteorShader.setVec3("shipPosition", glm::vec3(ship.spacePosition, 0.0));

			meteors[i].meteorShader.setMat4("rotation", rotation);
			meteors[i].meteorShader.setMat4("model", model);
			meteors[i].meteorShader.setMat4("projection", projection);
			meteors[i].meteorShader.setMat4("view", view);

			meteors[i].theMeteor.Draw(meteors[i].meteorShader);
		}

		//Calculate the matrices for the walls
	}

	void update() override {
		generate();

		std::vector<Meteor>::iterator it = meteors.begin();
		for (int i = 0; i < meteors.size(); i++) {
			//Calculate new position for the meteor
			if (meteors[i].Position.z > 3.0f || meteors[i].Position.y < -2.0) {
				it = meteors.erase(it); //removing the element
			}
			if (it != meteors.end()) {
				meteors[i].Position.z += 1 * speed * deltaTime;
				meteors[i].Position.y -= 3.0f * deltaTime;
				meteors[i].Rotation += glm::vec3(1.0) * deltaTime;
				//Rotation will also have to affect the normals!!

				//This will be implemented if it needs to have better collision capability
				// 
				//glm::mat4 temp = meteors[i].theMeteor.update(meteors[i].theMeteor.Position, meteors[i].theMeteor.Position, meteors[i].theMeteor.Scale);
				//meteors[i].theMeteor.update(meteors[i].Position, meteors[i].Rotation, meteors[i].Scale);
				//meteors[i].theMeteor.buildBoundingBox();
				//boundingBoxes.push_back(meteors[i].theMeteor.boundingBox);
				it++;
			}
		}

	}

private:


};
#pragma once