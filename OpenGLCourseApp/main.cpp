#include <stdio.h>>
#include <string.h>
#include <cmath>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>



// define window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;


// vertex Shader (will take the vertices so we can manipulate the values and pass them to the fragment shader)
// uniform variables are really nice for projection and model matricies and keeping things... uniform. 
static const char* vShader = "												\n\
#version 330																\n\
																			\n\
layout (location = 0) in vec3 pos;											\n\
																			\n\
out vec4 vCol;																\n\
																			\n\
uniform mat4 model;															\n\
																			\n\
void main()																	\n\
{																			\n\
	gl_Position = model * vec4(pos, 1.0);									\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);								\n\
}";																


// fragment shader
static const char* fShader = "												\n\
#version 330																\n\
																			\n\
in vec4 vCol;																			\n\
																			\n\
out vec4 colour;															\n\
																			\n\
void main()																	\n\
{																			\n\
	colour = vCol;										\n\
}";

void CreateTriangle()
{
	unsigned int indicies[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0, 
		0, 1, 2
	};

	//plot triangle points
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f, // extra point for indexing 
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// Creating a vertex array in gpu and get reference to VAO we give it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); // no bind the vao to the array

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO ); // element buffer stores indicies
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies,  GL_STATIC_DRAW); // bind index buffer object. 


	//Create buffer object inside vao
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//connect buffer data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // static draw as we wont change values in array. GL dynamic lets you do that though


	//
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	//  Create an empty shader of that type 
	GLuint theShader = glCreateShader(shaderType);

	//pointer directy to code
	const GLchar* theCode[1];
	theCode[0] = shaderCode; // pass code

	// length of code 
	GLint codeLegth[1]; 
	codeLegth[0] = strlen(shaderCode);

	//shader in memory will now have its code modified
	glShaderSource(theShader, 1, theCode, codeLegth);
	glCompileShader(theShader);

	GLint result = 0; 
	GLchar eLog[1024] = { 0 }; 

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog); 
		return;
	}

	//Attach Shader
	glAttachShader(theProgram, theShader); 

}
void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader) {
		printf("Error creating shader program!\n");
		return; // will return straight to main though there is nothing really else there 
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0; // result of g functions we will perform
	GLchar eLog[1024] = { 0 }; // place to log the errors.  Inellisense doesn't pick up on the shader codeso we have to write good code. 
	// detailed error logs is ESSENTIAL

	//Create executables on GPU
	glLinkProgram(shader);
	// Validate proper linkage 
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog); //print out log where error is, with help of some formatting 
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error vaidating program: '%s'\n", eLog); //print out log where error is, with help of some formatting 
		return;
	}
	// Grab location of the uniform variable by grabbing its ID manually
	uniformModel = glGetUniformLocation(shader, "model");
}

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

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
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
	
	glEnable(GL_DEPTH_TEST); // lets us find out which triangles are "deeper than others for depth testing);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events
		// check for any events like mouse clicks, window resizing etc
		glfwPollEvents();

		// everytime we go through the loop check direction
		if (direction)
		{
			// increase tri offset by increment value, if moving to right += if left -=
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}
		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}
		curAngle += 0.01f;
		if (curAngle >= 360)
		{
			// this is just to prevent possible overflow. 
			curAngle -= 360;
		}

		if (sizeDirection)
		{
			curSize += 0.0001f;
		}
		else {
			curSize -= 0.0001f;
		}

		if (curSize >= maxSize || curSize <= minSize) {
			sizeDirection = !sizeDirection;
		}

		// clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // each pixel has more data than just color like depth, here we are spcifying we want to clear all colors.
		// clear color buffer and depth buffer bits
		

		//Grab id and when we call use program it woudl use the one with id of shader
		// if we had multipel shaders it would draw them all and use the one we want but this program
		// currently only uses one shader
		glUseProgram(shader);

		// matrix with all values as 0 except bottom// essentially no calculations performed on it
		glm::mat4 model(1.0f);
		// output value of translate will be applied to model instead of doing changes directly on the model. 
		// be aware of order of transformationsg
		
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		
		//Scaling, note scaling happens relative to origin so we would get different results depending on if we scale before 
		// or after rotating, translating etc. 
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		//Assign value to shader itself
		// 1 = 1 value f = float
		// when shader is attached set uniform value to triOffset, which will change value of our uniform value. 
		glUniform1f(uniformModel, triOffset);

		// we need value pointer as model isn't directly in a format we can use. 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);// unbinding IBO

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}

	return 0;
}