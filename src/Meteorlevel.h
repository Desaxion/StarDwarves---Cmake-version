#pragma once
#include "Level.h"
#include "Meteor.h"
#include <vector>
#include <time.h>

class Meteorlevel : public Level
{
public: 

	int numberOfMeteorsLimit = 100;
	float spawInterval = 1.5;

	int prevSpawnTime = 0;

	std::vector<Meteor> meteors;

	void generate() override{
		//Make sure that the meteor is actually to be spawned. This will only spawn one meteor at a time.
		if(meteors.size() < numberOfMeteorsLimit && prevSpawnTime != runtimeMS()){
			//Make the spawn random

		Meteor newMet;
		glm::mat4 model = newMet.theMeteor.update(newMet.Position, newMet.Rotation, newMet.Scale);
		prevSpawnTime = runtimeMS();

		meteors.push_back(newMet);
		
		
			for(int i = 0; i < newMet.theMeteor.meshes.size(); i++){
				meshes.push_back(newMet.theMeteor.meshes[i]); //Adding the meshes of each meteor in to the level so that we can check for collision
			}
		
		}
	}

	void draw(glm::mat4 projection, glm::mat4 view, glm::vec4 metColor) override {
		for (int i = 0; i < meteors.size(); i++) {
			meteors[i].meteorShader.use();

			meteors[i].meteorShader.setVec4("metColor", metColor);

			//glm::mat4 model = meteors[i].theMeteor.update(meteors[i].Position, meteors[i].Position, meteors[i].Scale);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, meteors[i].Position);
			//std::cout << meteors[i].Position.z << "\n";
			model = glm::rotate(model, meteors[i].Rotation.x, glm::vec3(1, 0, 0));
			model = glm::rotate(model, meteors[i].Rotation.y, glm::vec3(0, 1, 0));
			model = glm::rotate(model, meteors[i].Rotation.z, glm::vec3(0, 0, 1));
			model = glm::scale(model, meteors[i].Scale);

			//meteors[i].meteorShader.setVec3("normal", meteors[i].theMeteor.meshes[0].)
			meteors[i].meteorShader.setMat4("model", model);
			meteors[i].meteorShader.setMat4("projection", projection);
			meteors[i].meteorShader.setMat4("view", view);
			meteors[i].theMeteor.Draw(meteors[i].meteorShader);
		}
	}
	
	void update() override {

		generate();
		boundingBoxes.clear();

		std::vector<Meteor>::iterator it = meteors.begin();
		for (int i = 0; i < meteors.size(); i++) {
			//Calculate new position for the meteor
			if (meteors[i].Position.z > 3.0f) {
				it = meteors.erase(it); //removing the element
			}
			if (it != meteors.end()) {
			meteors[i].Position.z += speed * deltaTime;
			meteors[i].Rotation += glm::vec3(1.0) * deltaTime;
		


			//glm::mat4 temp = meteors[i].theMeteor.update(meteors[i].theMeteor.Position, meteors[i].theMeteor.Position, meteors[i].theMeteor.Scale);
			meteors[i].theMeteor.update(meteors[i].Position, meteors[i].Rotation, meteors[i].Scale);
			meteors[i].theMeteor.buildBoundingBox();
			boundingBoxes.push_back(meteors[i].theMeteor.boundingBox);

			
				it++;
			}
			
		}


	}

private:


};