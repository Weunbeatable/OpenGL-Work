#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
}

// if user specifies values. 
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int Window::Initialise()
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

	 mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow) // if we weren't able to create the window for some reason
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffersize info, getting size of viewport which is the part that gets drawn
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight); // grab main window and references to buffer width and height.

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow); // if you even wanted you could have it draw the content to more than one window if your game had more than one

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();

	// initalise glew libraries  and functions they use
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		// want to destroy window first
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); // lets us find out which triangles are "deeper than others for depth testing);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}