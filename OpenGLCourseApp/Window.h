#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); } // main loop

	//grab keys from class
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void SwapBuffers() {glfwSwapBuffers(mainWindow);}


	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	// way to get keyboard keys. 1024 covers ascii keys and if a key is pressed, check the list to see if number is pressed, if it is handle camera as necessary and when releaseed set to false
	bool keys[1024];

	// Checking last coordinates and changed values every time cursour changes, comparison of coordinates of current to last will give inference
	// to the amount of change. That change is used to move raoudn the screen. 
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved; // to prevent weird camera flicking on program starting. 

	//GLFW uses a callback for when a key is pressed, will handle by window since its being handled inside a calss itself. 
	// Callbacks don't go to member functions of a class so the handlekeys will be static, since its not assigned to one object it wont know
	// what to be assigned true or false to.

	void creatCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

