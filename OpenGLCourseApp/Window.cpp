#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

// if user specifies values. 
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
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


	// Handle Key + Mouse Inputs
	creatCallbacks();
	// lcok cursor to screen
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	//Callback wont know which window called it, there isn't a way of knowing that with handlekeys so userpointer will help fix that issue. 
	// will take window and owner of window
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::creatCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}


GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}
GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

//hanlde keys is private 
void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	// casting windowuserponiter for window which will get the user pointer defined above, giving access to the keys
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); // access to window

	// if user has pressed escape key run below
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("Pressed: %d\n", key); // give me the number of the key
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("Pressed: %d\n", key);
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window)); // access to window

	if(theWindow->mouseFirstMoved)
		{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;  // values can be flipped to match coordinate system of liking

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	
	//printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}