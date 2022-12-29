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
		meteors.push_back(newMet);
		
		
			for(int i = 0; i < newMet.theMeteor.meshes.size(); i++){
				meshes.push_back(newMet.theMeteor.meshes[i]); //Adding the meshes of each meteor in to the level so that we can check for collision
			}
		
		}
	}

	void draw(glm::mat4 projection, glm::mat4 view) override {
			

		for (Meteor m : meteors) {
			m.meteorShader.use();
			glm::mat4 model = glm::mat4(1.0f);
	
			m.meteorShader.setMat4("model", model);
			m.meteorShader.setMat4("projection", projection);
			m.meteorShader.setMat4("view", view);
			m.theMeteor.Draw(m.meteorShader);
		}
	}
	
	void update() override {

	}

private:


};