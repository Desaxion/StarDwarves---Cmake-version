#pragma once
#include "Level.h"
#include "Meteor.h"
#include <vector>

class Meteorlevel : public Level
{
public: 
	std::vector<Meteor> meteors;

	void generate() override{
		//Make sure that the meteor is actually to be spawned. This will only spawn one meteor at a time.
		if(meteors.size() == 0){
		Meteor newMet;
		glm::mat4 model = newMet.theMeteor.update(newMet.Position, newMet.Rotation, newMet.Scale);

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

			glm::mat4 model = meteors[i].theMeteor.update(meteors[i].Position, meteors[i].Position, meteors[i].Scale);
			meteors[i].meteorShader.setMat4("model", model);
			meteors[i].meteorShader.setMat4("projection", projection);
			meteors[i].meteorShader.setMat4("view", view);
			meteors[i].theMeteor.Draw(meteors[i].meteorShader);
		}
	}
	
	void update() override {

		generate();
		boundingBoxes.clear();
		
		for (int i = 0; i < meteors.size(); i++) {
			//glm::mat4 temp = meteors[i].theMeteor.update(meteors[i].theMeteor.Position, meteors[i].theMeteor.Position, meteors[i].theMeteor.Scale);
			meteors[i].theMeteor.update(meteors[i].Position, meteors[i].Rotation, meteors[i].Scale);
			meteors[i].theMeteor.buildBoundingBox();
			boundingBoxes.push_back(meteors[i].theMeteor.boundingBox);
		}


	}

private:


};