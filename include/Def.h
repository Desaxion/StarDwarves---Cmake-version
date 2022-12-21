#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define MACOS false

const unsigned int DEFAULT_WIDTH  = 1000;
const unsigned int DEFAULT_HEIGHT = 1000;

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



	

}