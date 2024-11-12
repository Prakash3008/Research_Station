#pragma once

#include "stdio.h"


#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>

class Window
{
public:
	GLFWwindow* mainWindow;
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	void setTitle(std::string title) { glfwSetWindowTitle(mainWindow, title.c_str()); }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void setShouldClose(bool shouldClose) {
		glfwSetWindowShouldClose(mainWindow, shouldClose);
	}

	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};