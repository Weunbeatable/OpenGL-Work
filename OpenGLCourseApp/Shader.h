#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode); // pass an array of chars
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation); // pass fiels with shader and fragment info

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private: 
	GLuint shaderID, uniformProjection, uniformModel;

	void CompileShader(const char* vertexCode, const char* fragmentCode); // will convert from vertex code and copiled in fragment shader
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

