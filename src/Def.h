#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Ship.h"
#include "Level.h"
#include "Model.h"
#include <time.h>
#include <chrono> //milliseconds
#include <cstdint>

#define MACOS false

glm::vec3 lightDirection = glm::normalize(glm::vec3(2, 1, 0));

const float EPSILON = 0.00000001;

long long startMS;

time_t start;
time_t runtime() {
	return time(NULL) - start;
}

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

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		std::cout << "DEBUG. PLACE BREAKPOINT HERE!" << "\n";
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

//NOT WORKING PROPERLY. USE COLLISIONCHECK INSTEAD
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


			//start = glm::vec3(1.0f);

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
			int it = 0;

			for (int j = 0; j < hitBox.meshes.size(); j++) {
				//iterate across triangles, doing this by iterating across three vertices at a time.
				int intersectionsX = 0; //intersections along X-vector
				int intersectionsY = 0; //intersections along Y-vector
				int intersectionsZ = 0; //intersections along Z-vector

				it++;
				for (int v = 0; v < hitBox.meshes[j].vertices.size(); v += 3) {
				
					std::vector<glm::vec3> triangleVertices = { hitBox.meshes[j].vertices[v].Position,
																hitBox.meshes[j].vertices[v + 1].Position,	
																hitBox.meshes[j].vertices[v + 2].Position };
					//std::cout << hitBox.meshes[2].vertices[0].Position.x << "\n";
					if (it == 2) {
					
					}
					intersectionsX += mollerTrumbore(start, endX, triangleVertices);
					intersectionsY += mollerTrumbore(start, endY, triangleVertices);
					intersectionsZ += mollerTrumbore(start, endZ, triangleVertices);
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


bool collisionCheck(const Model& hitBox,  Level* theLevel) {
	for (int j = 0; j < theLevel->boundingBoxes.size(); j++) {
		for (int i = 0; i < hitBox.meshes.size(); i++) {
			for (int k = 0; k < hitBox.meshes[i].vertices.size(); k++) {
				if (theLevel->boundingBoxes[j].isInsideBoundingBox(hitBox.meshes[i].vertices[k].Position)) {
					return true;
				}
			}
		}
	}

	return false;

}



//The hitbox should really be integrated within the ship class but I cant get it to work rn.
//This is also probably wrong :(
void calculateHitbox(Model& hitBox, Shader& hitBoxShader, const glm::vec2& shipPosition,glm::vec3 shipAngles) {
	//Calculating shipangles

	//REDUNDAT, REPLACE WITH update() from model
	glm::mat4 model = hitBox.update(glm::vec3(shipPosition, 0.0f), shipAngles, glm::vec3(ship.shipScale));
	
	hitBoxShader.setMat4("model", model);
}


float truncatedTrig(float x) {
	return glm::fract(sin(x)*1000000000);
}

//Returns application runtime in ms
float runtimeMS() {
	//std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	auto time_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return time_since_epoch - startMS;
}





glm::vec2 random2(glm::vec2 st)
{
	st = glm::vec2(glm::dot(st, glm::vec2(127.1, 311.7)),
		glm::dot(st, glm::vec2(269.5, 183.3)));
	return glm::vec2(-1.0 + 2.0 * glm::fract(sin(st.s) * 43758.5453123), -1.0 + 2.0 * glm::fract(sin(st.t) * 43758.5453123));
}

// Gradient Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/XdXGW8
float noise(glm::vec2 st)
{
	glm::vec2 i = floor(st);
	glm::vec2 f = fract(st);

	glm::vec2 u = glm::vec2(f.s * f.s * (3.0 - 2.0 * f.s), f.t * f.t * (3.0 - 2.0 * f.t));

	return  glm::mix(glm::mix(glm::dot(random2(i + glm::vec2(0.0, 0.0)), f - glm::vec2(0.0, 0.0)),
			glm::dot(random2(i + glm::vec2(1.0, 0.0)), f - glm::vec2(1.0, 0.0)), u.x),
			glm::mix(glm::dot(random2(i + glm::vec2(0.0, 1.0)), f - glm::vec2(0.0, 1.0)),
			glm::dot(random2(i + glm::vec2(1.0, 1.0)), f - glm::vec2(1.0, 1.0)), u.x), u.y);
}

glm::vec3 random3(glm::vec3 st)
{
	st = glm::vec3(glm::dot(st, glm::vec3(127.1, 311.7, 543.21)),
		glm::dot(st, glm::vec3(269.5, 183.3, 355.23)),
		glm::dot(st, glm::vec3(846.34, 364.45, 123.65))); // Haphazard additional numbers by IR
	return glm::vec3( -1.0 + 2.0 * glm::fract(sin(st.x) * 43758.5453123), -1.0 + 2.0 * glm::fract(sin(st.y) * 43758.5453123), -1.0 + 2.0 * glm::fract(sin(st.z) * 43758.5453123));
}

// Gradient Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/XdXGW8
// Trivially extended to 3D by Ingemar
float gradientNoise(glm::vec3 st)
{
	glm::vec3 i = floor(st);
	glm::vec3 f = fract(st);

	glm::vec3 u = glm::vec3(f.x * f.x * (3.0 - 2.0 * f.x), f.y * f.y * (3.0 - 2.0 * f.y), f.z * f.z * (3.0 - 2.0 * f.z));

	return glm::mix(
		   glm::mix(glm::mix(glm::dot(random3(i + glm::vec3(0.0, 0.0, 0.0)), f - glm::vec3(0.0, 0.0, 0.0)),
		   glm::dot(random3(i + glm::vec3(1.0, 0.0, 0.0)), f - glm::vec3(1.0, 0.0, 0.0)), u.x),
		   glm::mix(glm::dot(random3(i + glm::vec3(0.0, 1.0, 0.0)), f - glm::vec3(0.0, 1.0, 0.0)),
		   glm::dot(random3(i + glm::vec3(1.0, 1.0, 0.0)), f - glm::vec3(1.0, 1.0, 0.0)), u.x), u.y),

		   glm::mix(glm::mix(glm::dot(random3(i + glm::vec3(0.0, 0.0, 1.0)), f - glm::vec3(0.0, 0.0, 1.0)),
		   glm::dot(random3(i + glm::vec3(1.0, 0.0, 1.0)), f - glm::vec3(1.0, 0.0, 1.0)), u.x),
		   glm::mix(glm::dot(random3(i + glm::vec3(0.0, 1.0, 1.0)), f - glm::vec3(0.0, 1.0, 1.0)),
	       glm::dot(random3(i + glm::vec3(1.0, 1.0, 1.0)), f - glm::vec3(1.0, 1.0, 1.0)), u.x), u.y), u.z

	);
}