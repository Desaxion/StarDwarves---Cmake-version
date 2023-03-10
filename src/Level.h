#pragma once
#include "Mesh.h"
#include "BoundingBox.h"
class Level
{
public:

	virtual void generate() = 0;

	virtual glm::vec3 getLevelColor() = 0;

	virtual glm::vec3 getLightDirection() = 0;

	virtual void draw(glm::mat4 projection, glm::mat4, glm::vec4 color, float dt) = 0;

	virtual void update() = 0;

	virtual float getAmbientScaling() = 0;

	float speed = 1.0f; //this should go faster if the player has played good

	void updateSpeed(float scalingFactor) {
		speed += speed * scalingFactor * 0.000002;
	}

	std::vector<Mesh> meshes; //Meshes inside the level

	std::vector<BoundingBox> boundingBoxes;

	//glm::vec3 shipLightDirection;

	glm::vec3 lightDirection;
	glm::vec3 lightColor;



private:


};