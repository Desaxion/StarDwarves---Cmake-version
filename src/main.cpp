#include "Def.h"
#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include "Camera.h"
#include "Ship.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Meteorlevel.h"
#include "Ditchlevel.h"

//Space texture image <a href="https://www.freepik.com/free-photo/milky-way-galaxy-night_13249998.htm#query=space&position=5&from_view=search&track=sph">Image by tawatchai07</a> on Freepik


const unsigned int WIDTH = DEFAULT_WIDTH;	//1000
const unsigned int HEIGHT = DEFAULT_HEIGHT; //1000

#define skybox


int main() {
	time(&start);//starts timer in s and ms
	startMS = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


	//Begin with instantiating the window
	glfwInit(); //Tell compiler to initialize GLFW for usage
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3); //These two lines are to make sure that the user uses OpenGL 3.3 or more, otherwise window will not open
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Tell GLFW that we use the core profile of OpenGL, and not immediate, thus granting us more access to inner functions

	#ifdef MACOS //If the user uses MACOS, set MACOS above to true. This line below has to be run in that case.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	#endif

	//Creating our window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Star Dwarves", NULL, NULL);
	if (!window) { 
		std::cout << "Failed to create window. " << std::endl;
		glfwTerminate(); //Destroy everything related to window
		return -1; //Exit program with code -1.
	}
	glfwMakeContextCurrent(window); //Make sure that OpenGL draws to this very window.

	//Initialize glad before calling OpenGL functions, since glad help managing OpenGL pointers to the GPU
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //We send the OpenGL function pointers adresses to glad, and glfw helps with finding correct functions based on the operating system
		std::cout << "Failed to initialize glad" << std::endl;
		return -1;
	}


	//We load in all of the different shaders here
	////LOADING IN SHIP
	Model shipModel("../assets/models/ship/ship.obj");
	Shader shipShader("../src/shaders/ship.vs", "../src/shaders/ship.fs");
	Shader engineShader("../src/shaders/shipEngine.vs", "../src/shaders/shipEngine.fs");

	ship.shipModel = &shipModel;
	ship.mainShader = &shipShader;

	ship.engineShader = &engineShader;

	Model skyBoxModel("../assets/models/skyBox/skyBoxModel.obj");
	Shader skyBoxShader("../src/shaders/skyBox.vs", "../src/shaders/skyBox.fs");

	Model reticleModel("../assets/models/reticle/reticlenew.obj");
	Shader reticleShader("../src/shaders/reticle.vs", "../src/shaders/reticle.fs");

	Model hitBoxModel = Model("../assets/models/shiphitbox/ShipHitBox.obj");
	Shader hitBoxShader("../src/shaders/hitbox.vs", "../src/shaders/hitbox.fs");
	//The hitboxmodel is included within the ship so that its position can be calculated at the same time as the ship.



	//Tell OpenGL first the position coordinates for lower left point of the viewport relative to the window, and then the size of the viewport
	glViewport(0, 0, WIDTH, HEIGHT); //Position coordinates for lower left corner go from 0 to 1

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //Make sure the viewport is resized when user resizes window
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Make sure mouse movement is enabled (DEBUG)
	glfwSetCursorPosCallback(window, mouse_callback); //Enable the callback for mouse movement. (When game is done, remove these lines)
	
	//We generate a triangle
	float vertices[] = {
		-0.5f,-0.5f, 0.0f,
		 0.5f,-0.5f, 0.0f,
		 0.0f, 0.5f, 0.0f
	};

	//We create a vertex buffer object, that can handle sending multiple vertices at once to the vertex shader. It helps with managing the memory on the GPU
	unsigned int VBO;
	glGenBuffers(1, &VBO); //Allocate memory on the GPU for the specified VBO

	unsigned int VAO; //Create a vertex array object. It is used to store pointers to all vertices, indices, buffers etc. related to a speceific mesh.
	glGenVertexArrays(1, &VAO); //Create memory for the speciefied Vertex Array objects.

	glBindVertexArray(VAO);

	//We set the buffer to continue listening after us sending vertices. GL_ARRAY_BUFFER is the target and type of our VBO.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//From now on, all buffer calls we make to the GL_ARRAY_BUFFER target will configure this specific buffer

	//We copy the data from our vertices into the buffer. We use 'sizeof' to specify the size in bytes of the data we are to send in
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
			The last argument specifies how the GPU is going to use the data:
			GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
			GL_STATIC_DRAW: the data is set only once and used many times.
			GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
	*/



	//Now it's time to specify for OpenGL how the vertex data is being sent, we have to do it manually
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/*EXPLANATION FOR glVertexAttribPointer
	The first parameter specifies which vertex attribute we want to configure. Remember that we specified the location of the position vertex attribute in the vertex shader with layout (location = 0). This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
	
	The next argument specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values.
	
	The third argument specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values).
	
	The next argument specifies if we want the data to be normalized. If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float. This is not relevant for us so we'll leave this at GL_FALSE.
	
	The fifth argument is known as the stride and tells us the space between consecutive vertex attributes. Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride. Note that since we know that the array is tightly packed (there is no space between the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed). Whenever we have more vertex attributes we have to carefully define the spacing between each vertex attribute but we'll get to see more examples of that later on.
	
	The last parameter is of type void* and thus requires that weird cast. This is the offset of where the position data begins in the buffer. Since the position data is at the start of the data array this value is just 0. We will explore this parameter in more detail later on
	*/

	//Game logic, selecting level and such
	Level* selectedLevel;
	Meteorlevel meteorlevel;
	Ditchlevel ditchlevel;

	//Maybe create a button in the loop to check which level to be selected
	//selectedLevel = &meteorlevel; //At this moment, selecting the meteor level
	selectedLevel = &ditchlevel; //At this moment, selecting the ditch level


	////////////    MAIN     RENDERING      LOOP    //////////////

	//Enabling depth test
	glEnable(GL_DEPTH_TEST);


	while(!glfwWindowShouldClose(window)) { //Check if window is instructed to close
		//We redraw screen every frame, thus we clear the screen in beginning of every loop iteration
		selectedLevel->updateSpeed(runtime());
		
	
		
		//for (auto bb : selectedLevel->boundingBoxes) {
		//std::cout << "(" << bb.getMin().x << "," << bb.getMin().y << "," << bb.getMin().z << "," << "), (" << bb.getMax().x << "," << bb.getMax().y << "," << bb.getMax().z << ")\n";
		//}



		glm::vec4 metColor = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);

		if (collisionCheck(hitBoxModel, selectedLevel)) {
			

			metColor = glm::vec4(0.7f, 0.3f, 0.2f,1.0f);

			

			//std::cout << "COLLISION\n";
		}
		else {
			//std::cout << "NO COLLISION\n";
			metColor = glm::vec4(0.8f, 0.7f, 0.2f,1.0f);
		}

		
		


		//calculating time variables
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Specify the color of the cleariness
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Specify which buffer we clear (the others are GL_STENCIL_BUFFER_BIT and GL_DEPTH_BUFFER_BIT)
		
		//glBindVertexArray(VAO);	//Specify upcoming objects to be rendered
		//glDrawArrays(GL_TRIANGLES,0,3);//Draw the object! Second argument stands for index of starting vertex, last argument for amount of vertices to be drawn.

		shipShader.use(); //Tell the render which shader to use for upcoming objects
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::vec3 shipAngles = ship.shipAngles();

		float viewamount = 0.025f;

		view = glm::translate(view, glm::vec3(shipAngles.y * viewamount, -shipAngles.x * viewamount, 0));

		view = glm::rotate(view, -shipAngles.x*viewamount, glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -shipAngles.y*viewamount, glm::vec3(0.0f, 1.0f, 0.0f));
		
		float time = (float)glfwGetTime();

		glm::vec3 shipPosition = glm::vec3(ship.calculateShipPosition(deltaTime), 0.0f);

		//std::cout << "(" << shipModel.meshes[2].vertices[0].Position.x << "," << shipModel.meshes[2].vertices[0].Position.y << "," << shipModel.meshes[2].vertices[0].Position.z << ")" << "\n";
		//std::cout << "(" << hitBoxModel.meshes[0].vertices[17].Position.x << "," << hitBoxModel.meshes[0].vertices[17].Position.y << "," << hitBoxModel.meshes[0].vertices[17].Position.z << ")" << "\n";
		//std::cout << "(" << shipAngles.x << ", " << shipAngles.y << ", " << shipAngles.z << ")\n";
	//	std::cout << "(" << hitBoxModel.Position.x << ", " << hitBoxModel.Position.y << ", " << hitBoxModel.Position.z << ")\n";
		//std::cout << "(" << shipPosition.x << ", " << shipPosition.y << ", " << shipPosition.z << ")\n";

		glm::mat4 model = shipModel.update(shipPosition, shipAngles, glm::vec3(ship.shipScale));
	
		ship.Draw(projection, view, lightDirection, model, runtimeMS()); //Using the abstraction of the draw function, so that different shaders for the same object can be used.


		hitBoxShader.use();
		calculateHitbox(hitBoxModel, hitBoxShader, shipPosition, shipAngles);
		hitBoxShader.setBool("showHitBox", showHitBox);
		hitBoxShader.setMat4("projection", projection);
		hitBoxShader.setMat4("view", view);
		hitBoxModel.Draw(hitBoxShader);


#ifdef skybox

		skyBoxShader.use();

		//no need to update this
		glm::mat4 skymodel = glm::mat4(1.0f);
		skymodel = glm::translate(skymodel, glm::vec3(0.0f, 0.0f, 0.0f));
		skyBoxShader.setMat4("projection", projection);
		skyBoxShader.setMat4("skymodel", skymodel);
		skyBoxShader.setMat4("view", view);
		skyBoxShader.setFloat("time", time);
		skyBoxModel.Draw(skyBoxShader);
#endif

#ifdef reticle
		reticleShader.use();
		
		//reticleTrans = glm::translate(reticleTrans, glm::vec3(ship.reticlePosition,-5.0f));
		glm::mat4 reticleTrans = reticleModel.update(glm::vec3(ship.reticlePosition, -5.0f),glm::vec3(0.0), glm::vec3(1.0f));
		reticleShader.setMat4("reticleTrans", reticleTrans);
		reticleShader.setMat4("projection", projection);
		reticleShader.setMat4("view", view);
		

		reticleModel.Draw(reticleShader);
#endif		
		
		//Process input
		//Render Level
		selectedLevel->update();

		selectedLevel->draw(projection, view, metColor);
		
		//Display title screen

		//view controls, highscore and such

		//PLAY
		// Select level ----> Lets go
		//Epic gaming
		
		glfwSwapBuffers(window); //Swaps buffers so it can begin writing to one buffer and reading from the other
		glfwPollEvents(); //Check if glfw events are triggered (such as mouse and keyboard input)
	
	}

	//Destroy all things related to glfw and close the program
	glfwTerminate();
	std::cout << "Application runtime: " << runtime() << " seconds. " << std::endl;

	return 0;
}

