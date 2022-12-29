#pragma once
#include <glm/glm.hpp>
#include "Model.h"
#include "Shader.h"

const float scale = 0.05;


class Ship
{
public:


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
		return glm::normalize(glm::vec3(reticlePosition, 5.0f) - glm::vec3(spacePosition, 0.0));
	}

	glm::vec3 shipAngles() {
		float turnRatio = 0.5f;

		float phi = glm::asin(reticlePosition.x / glm::distance(glm::vec3(reticlePosition, 5.0f), glm::vec3(spacePosition, 0.0)));//in x/z plane or around x axis
		float theta = glm::asin(reticlePosition.y / glm::distance(glm::vec3(reticlePosition, 5.0f), glm::vec3(spacePosition, 0.0)));//in y/z plane or around y axis
		
		float alpha = (spacePosition.x - reticlePosition.x)*turnRatio;

		return glm::vec3(phi, theta, alpha);
	}


	void calculateHitbox() {
		glm::mat4 model = glm::mat4(1.0f);

		glm::vec3 shipAngles = shipAngles();

		model = glm::translate(model, glm::vec3(ship.calculateShipPosition(deltaTime), 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, shipAngles.y, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -shipAngles.x, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, shipAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale));

		hitBoxShader.setMat4("projection", projection);
		hitBoxShader.setMat4("view", view);
		hitBoxShader.setMat4("model", model);
	}
	





	Shader hitBoxShader("../src/shaders/hitbox.vs", "../src/shaders/hitbox.fs");
	Model hitBox = Model("../assets/models/shiphitbox/ShipHitBox.obj");

private:


	glm::vec2 acceleration = glm::vec2(0, 0);
	glm::vec2 velocity = glm::vec2(0, 0);

	const float forcecoeff = 15.0f;
	const float dampingcoeff = 20.0f;

	const float movementBoundary = 2.0f;
    const float MovementSpeed = 6.0f;
    const float FlyingSpeed = 0; //Calculate this dependent on time spent playing.
};