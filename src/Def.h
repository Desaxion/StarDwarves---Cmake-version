#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Ship.h"
#include "Level.h"
#include "Model.h"


#define MACOS false


const float EPSILON = 0.00000001;


const unsigned int DEFAULT_WIDTH  = 1000;
const unsigned int DEFAULT_HEIGHT = 1000;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float fov = 45.0f;

bool showHitBox = false;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = DEFAULT_WIDTH / 2.0f;
float lastY = DEFAULT_HEIGHT / 2.0f;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Ship ship;

//Will resize the OpenGL viewport if the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) { 
	glViewport(0, 0, width, height); //Set the viewport to the specified arguments
}

//function for processing input to the window
void processInput(GLFWwindow* window) {
	//Press escape -> Exit application
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	//Keyboard
	//Movement
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		showHitBox = !showHitBox;
	}

	//move the ship
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		ship.manuvre(ship.RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		ship.manuvre(ship.LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		ship.manuvre(ship.UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		ship.manuvre(ship.DOWN, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_UP) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_PRESS) {
		ship.pushReticleBack();
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera.Front = glm::normalize(direction);
}




//Recycling MollerTrumbore from TNCG15
int mollerTrumbore(const glm::vec3 start, const glm::vec3 end, std::vector<glm::vec3> vertices) {


	glm::vec3 ray = glm::normalize(end - start);

	glm::vec3 E1 = glm::vec3(vertices[1].x, vertices[1].y, vertices[1].z) - glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z);
	glm::vec3 E2 = glm::vec3(vertices[2].x, vertices[2].y, vertices[2].z) - glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z);
	glm::vec3 h = glm::cross(ray, E2);


	double a = glm::dot(E1, h);


	if (a > -EPSILON && a < EPSILON) { //Ray is parallel to triangle
		return 0;
	}
	double f = 1.0 / a;
	glm::vec3 s = start - glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z);
	double u = f * glm::dot(s, h);

	if (u < 0.0 || u > 1.0) { return 0; }

	glm::vec3 q = glm::cross(s, E1);

	double v = f * glm::dot(ray, q);

	if (v < 0.0 || u + v > 1.0) { return 0; }
	double t = f * glm::dot(E2, q);
	if (t > EPSILON)
	{
		glm::vec3 temp = start + glm::vec3(ray.x * t, ray.y * t, ray.z * t);
		//intersectionPoint = glm::vec4(temp.x, temp.y, temp.z, 1.0);
		return 1;
	}


	else { return 0; }

}



//Checking for collisions between a hitbox and meshes in a specified level. Returns true if hitbox has collided with any item in the level.
bool checkCollision(const Model& hitBox, const Level& theLevel) {
	//Important to note: Every vertex in a mesh is counted three times, since it is shared by three traingles. It is probably possible to find a way to ounly count each vertex once, but I don't really have
	//the time or resources to do that.

	//We check if any of the vertices of the level is inside any of the hitboxes

	//iterate through every hitbox mesh and test if any of the vertices in the level is inside it.
	//Iterate across all meshes in the level
	for (int i = 0; i < theLevel.meshes.size(); i++) {
		//Iterate across all vertices in the mesh and create a really long vector for each axis, use mollerTrumbore algorithm to check if any of the vectors intersect the hitbox more than once
		for (int k = 0; k < theLevel.meshes[i].vertices.size(); k++) {
			//We iterate across all triangles on all meshes

			glm::vec3 start = theLevel.meshes[i].vertices[k].Position;

			glm::vec3 endX = glm::vec3(start) + glm::vec3(1000.0f, 0.0f ,0.0f);
			glm::vec3 endY = glm::vec3(start) + glm::vec3(0.0f, 1000.0f, 0.0f);
			glm::vec3 endZ = glm::vec3(start) + glm::vec3(0.0f, 0.0f, 1000.0f);

			/*
							   endZ
								^
								|
								|
								|
						  start	|
							  \ |
				   			   \|
								o--------------> endY
							   /
							  /
							 /
							/
						  |_ 
						endX
			
					We check if any of the vectors drawn above are intersected within the same hitbox mesh. 
					If intersected once -> hit.
					If intersected twice -> No hit.

			*/

			for (int j = 0; j < hitBox.meshes.size(); j++) {
				//iterate across triangles, doing this by iterating across three vertices at a time.
				int intersectionsX = 0; //intersections along X-vector
				int intersectionsY = 0; //intersections along Y-vector
				int intersectionsZ = 0; //intersections along Z-vector

				for (int v = 0; v < hitBox.meshes[j].vertices.size(); v += 3) {

					std::vector<glm::vec3> vertices = { hitBox.meshes[j].vertices[v].Position,
														hitBox.meshes[j].vertices[v + 1].Position,	
														hitBox.meshes[j].vertices[v + 2].Position };
					
					intersectionsX += mollerTrumbore(start, endX, vertices);
					intersectionsY += mollerTrumbore(start, endY, vertices);
					intersectionsZ += mollerTrumbore(start, endZ, vertices);
				}

				if (intersectionsX % 2 == 1) {
					return true;
				}
				if (intersectionsY % 2 == 1) {
					return true;
				}
				if (intersectionsZ % 2 == 1) {
					return true;
				}
			}
		}
	}
	return false;
}

