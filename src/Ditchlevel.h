#pragma once
#include "Level.h"
#include "Ditch.h"
#include <vector>
#include <time.h>

class Ditchlevel : public Level
{
public:
	
	std::vector<Ditch> ditchv;

	void generate() override {
		int time = runtime();

		// Create the ditch and put it in a vector (ditchv)
		Ditch newDitch;
		ditchv.push_back(newDitch);

		// Push the vector into Level so it can draw the mesh
		meshes.push_back(newDitch.theDitch.meshes[0]);

		/*
			Add planes, rotate, translate, 
		*/

	}

	void draw(glm::mat4 projection, glm::mat4 view, glm::vec4 metColor) override {
		ditchv[0].ditchShader.use();
		ditchv[0].ditchShader.setVec4("metColor", metColor);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, ditchv[0].Position);
		//std::cout << meteors[i].Position.z << "\n";
		model = glm::rotate(model, ditchv[0].Rotation.x, glm::vec3(1, 0, 0));
		model = glm::rotate(model, ditchv[0].Rotation.y, glm::vec3(0, 1, 0));
		model = glm::rotate(model, ditchv[0].Rotation.z, glm::vec3(0, 0, 1));
		model = glm::scale(model, ditchv[0].Scale);

		glm::mat4 rotation = glm::mat4(1.0f);
		rotation = glm::rotate(rotation, ditchv[0].Rotation.x, glm::vec3(1, 0, 0));
		rotation = glm::rotate(rotation, ditchv[0].Rotation.y, glm::vec3(0, 1, 0));
		rotation = glm::rotate(rotation, ditchv[0].Rotation.z, glm::vec3(0, 0, 1));

		// Load into shaders
		ditchv[0].ditchShader.setVec3("ditchPosition", ditchv[0].Position);
		ditchv[0].ditchShader.setVec3("shipDirection", ship.shipDirection());
		ditchv[0].ditchShader.setVec3("lightDirection", lightDirection);
		ditchv[0].ditchShader.setVec3("shipPosition", glm::vec3(ship.spacePosition, 0.0));

		ditchv[0].ditchShader.setMat4("rotation", rotation);
		ditchv[0].ditchShader.setMat4("model", model);
		ditchv[0].ditchShader.setMat4("projection", projection);
		ditchv[0].ditchShader.setMat4("view", view);

		ditchv[0].theDitch.Draw(ditchv[0].ditchShader);
	}

	void update() override {
		generate();

	}

private:


}; 
