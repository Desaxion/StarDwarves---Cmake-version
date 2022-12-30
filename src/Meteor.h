#pragma once
#include "Model.h"

class Meteor
{
public:
	Meteor() {
	//Generate random shape
	//Generate random position and rotation speed
	};
	glm::vec3 Position = glm::vec3(truncatedTrig(runtimeMS()), truncatedTrig(runtimeMS() + 44), -10.0f); //random position, this is just for debug
	glm::vec3 Rotation = glm::vec3(truncatedTrig(runtimeMS()), truncatedTrig(runtimeMS()), truncatedTrig(runtimeMS())); //Rotation around its own axis
	glm::vec3 Scale = glm::vec3(1.0f);

	float displacementVelocity = 0;

	//Model theMeteor = Model("../assets/models/meteoritebase/meteoritebase.obj");
	Model theMeteor = Model("../assets/models/box/box.obj");
	Shader meteorShader = Shader("../src/shaders/meteorite.vs", "../src/shaders/meteorite.fs");

private:
	


};
