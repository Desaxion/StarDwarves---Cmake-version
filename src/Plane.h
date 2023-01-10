#pragma once
#include "Model.h"
#include "noise/noise1234.h"

class Plane {
public:

	Model thePlane = Model("../assets/models/plane/plane.obj");
	Shader planeShader = Shader("../src/shaders/meteorite.vs", "../src/shaders/meteorite.fs");
	
	Plane() {};
	
	Plane(const char* vertexPath, const char* fragmentPath) {
		planeShader = Shader(vertexPath, fragmentPath);		
	};

	glm::vec3 Position = glm::vec3(0.0, -0.8, 0.0f);//
	glm::vec3 Rotation = glm::vec3(0.0, 0.0, 0.0);// 
	glm::vec3 Scale = glm::vec3(0.1);//
	

};