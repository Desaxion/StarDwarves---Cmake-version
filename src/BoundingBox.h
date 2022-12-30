#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <string>
#include <array>


class BoundingBox
{
public:

	BoundingBox() = default;

	BoundingBox(glm::vec3 min, glm::vec3 max) {
		coords[0] = min;
		coords[1] = max;
	}

	//Checks if point is inside the bounding box
	bool isInsideBoundingBox(glm::vec3 point) {
		return ((point.x >= getMin().x && point.x <= getMax().x) && (point.y >= getMin().y && point.y <= getMax().y) && (point.z >= getMin().z && point.z <= getMax().z));
	}

	void setMin(glm::vec3 min) {
		coords[0] = min;
	}

	void setMax(glm::vec3 max) {
		coords[1] = max;
	}

	glm::vec3 getMin() {
		return coords[0];
	}

	glm::vec3 getMax() {
		return coords[1];
	}
	
private:

	std::array<glm::vec3, 2> coords = { glm::vec3(0.0f),  glm::vec3(0.0f) };

};