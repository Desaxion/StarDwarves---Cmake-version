#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Model.h"
#include "Shader.h"

const float scale = 0.05;


class Ship
{
public:

	const float turnRatio = 0.5f;

	enum Movement{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};


	Ship() {	
	};

	glm::vec2 spacePosition = glm::vec2(0, 0);
	glm::vec2 reticlePosition = glm::vec2(0, 0);
	
	void Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 lightDirection, glm::vec3 levelColor, glm::mat4 model, float runtime, float ambientScaling) {
		//Draw the ship using its main shader, instead of the engines
		
		int engineIndex0 = 55;
		int engineIndex1 = 58;
		int engineIndex2 = 61;

		mainShader->use();

		mainShader->setMat4("projection", projection);
		mainShader->setMat4("view", view);
		mainShader->setVec3("lightDirection", lightDirection);
		mainShader->setMat4("model", model);
		mainShader->setVec3("lightColor", levelColor);
		mainShader->setFloat("ambientScaling", ambientScaling);


		//could have done the following using if-statements instead, but wanted a more streamlined process (Not sure if good though)
		for (int i = 0; i < engineIndex0; i++) {
			shipModel->meshes[i].Draw(*mainShader);
		}

		for (int i = engineIndex0 + 1; i < engineIndex1; i++) {
			shipModel->meshes[i].Draw(*mainShader);
		}

		for (int i = engineIndex1 + 1; i < engineIndex2; i++) {
			shipModel->meshes[i].Draw(*mainShader);
		}

		for (int i = engineIndex2 + 1; i < shipModel->meshes.size(); i++) {
			shipModel->meshes[i].Draw(*mainShader);
		}


		engineShader->use();

		engineShader->setMat4("projection", projection);
		engineShader->setMat4("view", view);
		engineShader->setVec3("lightDirection", lightDirection);
		engineShader->setMat4("model", model);
		engineShader->setFloat("time", runtime);

		shipModel->meshes[engineIndex0].Draw(*engineShader);
		shipModel->meshes[engineIndex1].Draw(*engineShader);
		shipModel->meshes[engineIndex2].Draw(*engineShader);
	}
//#define invertYAxis
	void manuvre(Movement direction, float deltaTime) {
        float v = MovementSpeed * deltaTime;
	
			if (direction == LEFT){
				reticlePosition.x -= v;
				if (abs(reticlePosition.x) > movementBoundary) {
					reticlePosition.x = -(movementBoundary - 0.01f);
				}
			}
			if (direction == RIGHT) {
				reticlePosition.x += v;
				if (abs(reticlePosition.x) > movementBoundary) {
					reticlePosition.x = (movementBoundary - 0.01f);
				}
			}

			if (direction == UP) {

				#ifndef invertYAxis
				reticlePosition.y += v;
				if (abs(reticlePosition.y) > movementBoundary) {
					reticlePosition.y = (movementBoundary - 0.01f);
				}
				#else
				reticlePosition.y -= v;
				if (abs(reticlePosition.y) > movementBoundary) {
					reticlePosition.y = -(movementBoundary - 0.01f);
			}
				#endif
				
				
			
			}
			if (direction == DOWN) {
				#ifndef invertYAxis
				reticlePosition.y -= v;
				if (abs(reticlePosition.y) > movementBoundary) {
					reticlePosition.y = -(movementBoundary - 0.01f);
				}
				#else
				reticlePosition.y += v;
				if (abs(reticlePosition.y) > movementBoundary) {
					reticlePosition.y = (movementBoundary - 0.01f);
				}
				#endif



			
			}
	


	}

	void pushReticleBack() {
		float ratio = 0.05f;
		reticlePosition = reticlePosition - ratio * (reticlePosition);
	}

#define reticleFlight
	//calculate the ship acceleration towards the reticle?
#ifdef reticleFlight
// calculate movement based on eulers formula:
//	
// v = v0 + at
// p = po + vt
//
	glm::vec2 calculateShipPosition(float deltaTime) {
		acceleration = forcecoeff * (reticlePosition - spacePosition) - dampingcoeff * velocity;
		if (spacePosition.x > movementBoundary - 0.5) {
			acceleration.x = -forcecoeff;
		}
		if (spacePosition.x < -movementBoundary + 0.5) {
			acceleration.x = forcecoeff;
		}
		if (spacePosition.y > movementBoundary - 0.5) {
			acceleration.y = -forcecoeff;
		}
		if (spacePosition.y < -movementBoundary + 0.5) {
			acceleration.y = forcecoeff;
		}

		velocity = velocity + acceleration * deltaTime;
		
		
		return spacePosition = spacePosition + velocity * deltaTime;
	}

#endif

	glm::vec3 shipDirection() {
		glm::mat4 rot = glm::mat4(1.0f);

		glm::vec3 rotation = shipAngles();
		rot = glm::rotate(rot, rotation.x, glm::vec3(1, 0, 0));
		rot = glm::rotate(rot, rotation.y, glm::vec3(0, 1, 0));
		rot = glm::rotate(rot, rotation.z, glm::vec3(0, 0, 1));

		glm::vec4 temp = glm::vec4(direction, 1.0);

		temp = rot*temp;

		return glm::normalize(temp);
	}

	glm::vec3 shipAngles() {
		

		float phi = glm::asin(reticlePosition.x / glm::distance(glm::vec3(reticlePosition, 5.0f), glm::vec3(spacePosition, 0.0)));//in x/z plane or around x axis
		float theta = glm::asin(reticlePosition.y / glm::distance(glm::vec3(reticlePosition, 5.0f), glm::vec3(spacePosition, 0.0)));//in y/z plane or around y axis
		
		float alpha = (spacePosition.x - reticlePosition.x)*turnRatio;

		return glm::vec3(theta, -phi, alpha);
	}

	Shader* mainShader;
	Shader* engineShader;
	Model* shipModel;

	glm::vec3 shipScale = glm::vec3(0.05f);

private:
	glm::vec3 direction = glm::vec3(0, 0, -1); //Ship is facing negative x-axis.

	glm::vec2 acceleration = glm::vec2(0, 0);
	glm::vec2 velocity = glm::vec2(0, 0);

	const float forcecoeff = 15.0f;
	const float dampingcoeff = 20.0f;

	const float movementBoundary = 2.0f;
    const float MovementSpeed = 6.0f;
    const float FlyingSpeed = 0; //Calculate this dependent on time spent playing.
};