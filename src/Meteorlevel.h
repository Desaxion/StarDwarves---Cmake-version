#pragma once
#include "Level.h"
#include "Meteor.h"
#include <vector>

class Meteorlevel : public Level
{
public: 
	std::vector<Meteor> meteors;

	void generate() override{
		//Make sure that the meteor is actually to be spawned
		if(meteors.size() == 0){
		Meteor newMet;
		meteors.push_back(newMet);
		}
	}

	void draw() override {
		for (Meteor m : meteors) {
			glm::mat4 model = glm::mat4(1.0f);
		
			m.meteorShader.setMat4("model", model);
			//model.setMat4("projection", projection);
			//reticleShader.setMat4("view", view);
			m.theMeteor.Draw(m.meteorShader);
		}
	}
	
	void update() override {

	}

private:


};