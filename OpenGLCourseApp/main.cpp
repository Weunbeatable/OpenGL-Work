#include <stdio.h>>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

const float toRadians = 3.14159265f / 180.0f;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;



// vertex Shader (will take the vertices so we can manipulate the values and pass them to the fragment shader)
// uniform variables are really nice for projection and model matricies and keeping things... uniform. 
static const char* vShader = "Shaders/shader.vert";																


// fragment shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
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

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indicies, 12, 12); // the verticies and indices will be taken from a value later but for testing they now use static values.
	meshList.push_back(obj1); // add to end of list

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indicies, 12, 12); // the verticies and indices will be taken from a value later but for testing they now use static values.
	meshList.push_back(obj2); // add to end of list
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;
	// projection is above while because we don't currently need to constantly be chaning the value
	// using a standard fov of 45, width and height of screen as an aspect ratio e.g. 4:3 then projection values. 
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get + Handle user input events
		// check for any events like mouse clicks, window resizing etc
		glfwPollEvents();



		// clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // each pixel has more data than just color like depth, here we are spcifying we want to clear all colors.
		// clear color buffer and depth buffer bits
		

		//Grab id and when we call use program it woudl use the one with id of shader
		// if we had multipel shaders it would draw them all and use the one we want but this program
		// currently only uses one shader
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		// matrix with all values as 0 except bottom// essentially no calculations performed on it
		glm::mat4 model(1.0f);
		// output value of translate will be applied to model instead of doing changes directly on the model. 
		// be aware of order of transformationsg
		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		
		//Scaling, note scaling happens relative to origin so we would get different results depending on if we scale before 
		// or after rotating, translating etc. 
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		//Assign value to shader itself
		// 1 = 1 value f = float
		// when shader is attached set uniform value to triOffset, which will change value of our uniform value. 
		//glUniform1f(uniformModel, triOffset);

		// we need value pointer as model isn't directly in a format we can use. 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();


		glUseProgram(0);

		mainWindow.SwapBuffers();

	}

	return 0;
}