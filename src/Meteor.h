#pragma once
#include "Model.h"

class Meteor
{
public:
	Meteor() {
	//Generate random shape
	//Generate random position and rotation speed
	};
	glm::vec3 Position = glm::vec3(0.0f,0.0f, 0.0f); //random position, this is just for debug
	glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f); //Rotation around its own axis
	glm::vec3 Scale = glm::vec3(2.0f);

	//Model theMeteor = Model("../assets/models/meteoritebase/meteoritebase.obj");
	Model theMeteor = Model("../assets/models/box/box.obj");
	Shader meteorShader = Shader("../src/shaders/meteorite.vs", "../src/shaders/meteorite.fs");

private:
	


};
