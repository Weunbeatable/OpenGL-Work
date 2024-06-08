#include <stdio.h>>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// define window dimensions
const GLint WIDTH = 800, HEIGHT = 600;


int main()
{
	//Initalise GLFW
	if (!glfwInit())
	{
		printf("GLFW Initalisation failed!");
		glfwTerminate(); // close everything down
		return 1; // let us know the program has exited in a way that it broke/crashed
	}


	//Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL context the window will have will be version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // same for minor 
	// Core profile = No Backwards compatibility 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	// Allow forward compatiblity 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow) // if we weren't able to create the window for some reason
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffersize info, getting size of viewport which is the part that gets drawn
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // grab main window and references to buffer width and height.

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow); // if you even wanted you could have it draw the content to more than one window if your game had more than one

	// Allow modern extension features
	glewExperimental = GL_TRUE; 

	// initalise glew libraries  and functions they use
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW intialisation failed");
		// want to destroy window first
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events
		// check for any events like mouse clicks, window resizing etc
		glfwPollEvents();

		// clear window
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // each pixel has more data than just color like depth, here we are spcifying we want to clear all colors.

		glfwSwapBuffers(mainWindow);

	}

	return 0;
}