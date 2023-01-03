#pragma once
#include "Mesh.h"
#include "BoundingBox.h"
class Level
{
public:

	virtual void generate() = 0;

	virtual void draw(glm::mat4 projection, glm::mat4 , glm::vec4 color) = 0;

	virtual void update() = 0;

	float speed = 1.0f; //this should go faster if the player has played good

	void updateSpeed(float scalingFactor) {
		speed += speed * scalingFactor * 0.000002;
	}

	std::vector<Mesh> meshes; //Meshes inside the level

	std::vector<BoundingBox> boundingBoxes;

private:


};