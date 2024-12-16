#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include "commonFns.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------
#include "Headers/Model.h"
#include "Headers/Mesh.h"
#include "Headers/Camera.h"
#include "Headers/Window.h"
#include "Headers/shaderClass.h"
#include "Headers/StationMesh.h"
#include "Headers/StationTexture.h"

Window mainWindow;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat curAngle = 0.0f;
GLfloat blackhawkAngle = 0.0f;
const float toRadians = 3.14159265f / 180.0f;

float modelPositionZ = 0.0f;   // Position along the Z-axis
float movementSpeed = 1.0f;    // Movement speed
float rotationAngle = 0.0f;    // Rotation angle in degrees
float rotationSpeed = 180.0f;   // Rotation increment per reversal (degrees)
float upperBound = 100.0f;
float lowerBound = -100.0f;

std::vector<StationMesh*> meshList;
StationTexture floorTexture;
StationTexture wallTexture;



GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;


float skyboxVertices[] =
{
	//   Coordinates
	-100.0f, -100.0f,  100.0f,//        7--------6
	 100.0f, -100.0f,  100.0f,//       /|       /|
	 100.0f, -100.0f, -100.0f,//      4--------5 |
	-100.0f, -100.0f, -100.0f,//      | |      | |
	-100.0f,  100.0f,  100.0f,//      | 3------|-2
	 100.0f,  100.0f,  100.0f,//      |/       |/
	 100.0f,  100.0f, -100.0f,//      0--------1
	-100.0f,  100.0f, -100.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void createStation() {
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	unsigned int wallIndices[] = {
	0, 2, 1,
	1, 2, 3
	};

	GLfloat floorVertices[] = {
		// Floor
		-20.0f, 0.0f, -10.0f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
		 20.0f, 0.0f, -10.0f,  20.0f, 0.0f,  0.0f, -1.0f, 0.0f,
		-20.0f, 0.0f, 10.0f,   0.0f, 10.0f,  0.0f, -1.0f, 0.0f,
		 20.0f, 0.0f, 10.0f,   20.0f, 10.0f, 0.0f, -1.0f, 0.0f
	};

	GLfloat wallVertices[] = {
		// Front Wall
		-20.0f, 0.0f, 10.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
		 20.0f, 0.0f, 10.0f,   20.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		-20.0f, 10.0f, 10.0f,  0.0f, 10.0f,  0.0f, 0.0f, 1.0f,
		 20.0f, 10.0f, 10.0f,  20.0f, 10.0f, 0.0f, 0.0f, 1.0f,

		 // Back Wall
		 -20.0f, 0.0f, -10.0f,  0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
		  20.0f, 0.0f, -10.0f,  20.0f, 0.0f,  0.0f, 0.0f, -1.0f,
		 -20.0f, 10.0f, -10.0f, 0.0f, 10.0f,  0.0f, 0.0f, -1.0f,
		  20.0f, 10.0f, -10.0f, 20.0f, 10.0f, 0.0f, 0.0f, -1.0f,

		  // Left Wall
		  -20.0f, 0.0f, -10.0f,  0.0f, 0.0f,   -1.0f, 0.0f, 0.0f,
		  -20.0f, 0.0f, 10.0f,   20.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
		  -20.0f, 10.0f, -10.0f, 0.0f, 10.0f,  -1.0f, 0.0f, 0.0f,
		  -20.0f, 10.0f, 10.0f,  20.0f, 10.0f, -1.0f, 0.0f, 0.0f,

		  // Right Wall
		   20.0f, 0.0f, -10.0f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		   20.0f, 0.0f, 10.0f,   20.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		   20.0f, 10.0f, -10.0f, 0.0f, 10.0f,  1.0f, 0.0f, 0.0f,
		   20.0f, 10.0f, 10.0f,  20.0f, 10.0f, 1.0f, 0.0f, 0.0f
	};

	GLfloat roofVertices[] = {
		// Roof
		-20.0f, 10.0f, -10.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f, 
		 20.0f, 10.0f, -10.0f, 20.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		-20.0f, 10.0f, 10.0f,  0.0f, 10.0f,  0.0f, 1.0f, 0.0f,
		 20.0f, 10.0f, 10.0f,  20.0f, 10.0f, 0.0f, 1.0f, 0.0f
	};

	StationMesh* floor = new StationMesh();
	floor->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(floor);

	for (int i = 0; i < 4; i++) { // Add all walls (front, back, left, right)
		StationMesh* wall = new StationMesh();
		wall->CreateMesh(&wallVertices[i * 32], wallIndices, 32, 6);
		meshList.push_back(wall);
	}

	StationMesh* roof = new StationMesh();
	roof->CreateMesh(roofVertices, wallIndices, 32, 6);
	meshList.push_back(roof);
}

void RenderStation(unsigned int shaderID) {
	glUniform1i(glGetUniformLocation(shaderID, "matTexture"), 1);

	glm::mat4 model;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(10.0f, -5.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	floorTexture.UseStationTexture();
	meshList[0]->RenderMesh();

	for (int i = 1; i <= 4; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wallTexture.UseStationTexture();
		meshList[i]->RenderMesh();
	}


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	// Set transparency uniform
	int transparencyLocation = glGetUniformLocation(shaderID, "transparency");
	glUniform1f(transparencyLocation, 0.6f); // 50% transparency

	// Disable depth mask for blending
	

	floorTexture.UseStationTexture(); // Assuming you're using the floor texture for the roof
	meshList[5]->RenderMesh();

	// Re-enable depth mask
	glDepthMask(GL_TRUE);

	// Disable blending after rendering
	glDisable(GL_BLEND);
}




int main()
{
	mainWindow = Window(1280, 768); 
	mainWindow.Initialise();

	createStation();

	floorTexture = StationTexture("Resources/Textures/floor.jpg");
	floorTexture.LoadStationTexture();
	wallTexture = StationTexture("Resources/Textures/wall.jpg");
	wallTexture.LoadStationTexture();

	camera = Camera(glm::vec3(0.0f, -95.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 5.0f, 0.5f);

	// Generates Shader objects
	Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
	Shader skyboxShader("Shaders/skybox.vert", "Shaders/skybox.frag");
	Shader stationShader("Shaders/station.vert", "Shaders/station.frag");

	//fish = Model();
	//fish.LoadModel("Resources/Models/Fish/fish.obj");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.Activate();
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);


	
	// Variables to create periodic event for FPS displaying
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;

	// Use this to disable VSync (not advized)
	//glfwSwapInterval(0);


	// Create VAO, VBO, and EBO for the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// All the faces of the cubemap (make sure they are in this exact order)
	std::string facesCubemap[6] =
	{
		"Resources/Textures/Underwater/px.jpg",
		"Resources/Textures/Underwater/nx.jpg",
		"Resources/Textures/Underwater/py.jpg",
		"Resources/Textures/Underwater/ny.jpg",
		"Resources/Textures/Underwater/pz.jpg",
		"Resources/Textures/Underwater/nz.jpg"
	};


	// Creates the cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}


	Model model("Resources/Models/Fish/fish.obj");
	// Create transformation matrix for the model
	

	Model seahorse_model("Resources/Models/Seahorse/Seahorse.obj");

	Model chair1("Resources/Models/Chair/Chair.obj");

	Model table1("Resources/Models/Table/table.obj");

	Model gas("Resources/Models/Gas/oxygen_cylinder.glb");


	//Model coral("Resources/Models/Coral/10010_Coral_v1.obj");

	


	// Main while loop
	while (!mainWindow.getShouldClose())
	{
		// Updates counter and times
		crntTime = glfwGetTime();
		deltaTime = crntTime - prevTime;
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0)
		{
			
			// Resets times and counter
			prevTime = crntTime;
			counter = 0;

			// Use this if you have disabled VSync
			//camera.Inputs(window);
		}

		// Take care of all GLFW events
		glfwPollEvents();
		glDisable(GL_CULL_FACE); // Disable culling for transparency
		glEnable(GL_DEPTH_TEST);
		

		

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		



		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update position for linear movement
		modelPositionZ += movementSpeed * deltaTime;

		// Check bounds and reverse direction, add rotation
		if (modelPositionZ > upperBound) {
			movementSpeed = -1.0f;              // Reverse direction
			rotationAngle += rotationSpeed;    // Rotate model
		}
		if (modelPositionZ < lowerBound) {
			movementSpeed = 1.0f;              // Reverse direction
			rotationAngle += rotationSpeed;    // Rotate model
		}
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, modelPositionZ)); // Translate

		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate

		//glm::mat4 fishMatrix = glm::mat4(1.0f);
		//fishMatrix = glm::translate(fishMatrix, glm::vec3(0.0f, sin(glfwGetTime()) * 0.5f, 0.0f)); // Oscillate up and down



		//glm::mat4 projection = glm::perspective(glm::radians(90.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.01f, 100.0f);
		glm::mat4 projection = camera.calculateProjectionMatrix((float)mainWindow.getBufferWidth() / mainWindow.getBufferHeight());


		glm::mat4 view = camera.calculateViewMatrix();
		

		// Pass these matrices to your shader
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		model.Draw(shaderProgram.ID);

		glm::mat4 coral_transform = glm::mat4(1.0f);

		coral_transform = glm::translate(coral_transform, glm::vec3(10.0f, -100.0f, 0.0f)); // Translate

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(coral_transform));


		//coral.Draw(shaderProgram.ID);

		glm::mat4 seahorseModel = glm::mat4(1.0f);

		seahorseModel = glm::translate(seahorseModel, glm::vec3(10.0f, -20.0f, modelPositionZ)); // Translate
		seahorseModel = glm::rotate(seahorseModel, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(seahorseModel));


		seahorse_model.Draw(shaderProgram.ID);

		glm::mat4 tableModel = glm::mat4(1.0f);

		tableModel = glm::translate(tableModel, glm::vec3(27.0f, -98.5f, 0.0f)); // Translate

		tableModel = glm::scale(tableModel, glm::vec3(0.03f, 0.03f, 0.03f));

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(tableModel));

		table1.Draw(shaderProgram.ID);

		glm::mat4 chairModel = glm::mat4(1.0f);

		chairModel = glm::translate(chairModel, glm::vec3(23.0f, -100.0f, 0.0f)); // Translate
		chairModel = glm::rotate(chairModel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate

		chairModel = glm::scale(chairModel, glm::vec3(0.12f, 0.12f, 0.12f));

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(chairModel));

		chair1.Draw(shaderProgram.ID);

		glm::mat4 gasModel = glm::mat4(1.0f);

		gasModel = glm::translate(gasModel, glm::vec3(27.0f, -98.1f, 8.0f)); // Translate
		gasModel = glm::rotate(gasModel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate

		gasModel = glm::scale(gasModel, glm::vec3(0.02f, 0.02f, 0.02f));

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(gasModel));

		gas.Draw(shaderProgram.ID);


		// Handles camera inputs (delete this if you have disabled VSync)
		// Updates and exports the camera matrix to the Vertex Shader

		// Draw the normal model
		//model.Draw(shaderProgram, camera);

		// Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
		glDepthFunc(GL_LEQUAL);

		


		skyboxShader.Activate();
		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// The last row and column affect the translation of the skybox (which we don't want to affect)
		//view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		// projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		stationShader.Activate();
		glm::mat4 station_floor = glm::mat4(1.0f);

		station_floor = glm::translate(station_floor, glm::vec3(10.0f, -100.0f, 0.0f)); // Translate
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(station_floor));
		glUniformMatrix4fv(glGetUniformLocation(stationShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(stationShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		RenderStation(stationShader.ID);

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);


		// Swap the back buffer with the front buffer
		mainWindow.swapBuffers();

	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	skyboxShader.Delete();

	return 0;
}