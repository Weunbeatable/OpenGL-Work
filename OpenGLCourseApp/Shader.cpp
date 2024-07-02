#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str(); // convert to const char array
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation) // will luse some file I/O
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read%s| file doesn't exist.", fileLocation);
		return"";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n"); //gsl isn't whitespace sensetive but this will make the code cleaner in command line debugging. 
	}

	// close filestream
	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID) {
		printf("Error creating shader program!\n");
		return; // will return straight to main though there is nothing really else there 
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0; // result of g functions we will perform
	GLchar eLog[1024] = { 0 }; // place to log the errors.  Inellisense doesn't pick up on the shader codeso we have to write good code. 
	// detailed error logs is ESSENTIAL

	//Create executables on GPU
	glLinkProgram(shaderID);
	// Validate proper linkage 
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog); //print out log where error is, with help of some formatting 
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error vaidating program: '%s'\n", eLog); //print out log where error is, with help of some formatting 
		return;
	}
	// Grab location of the uniform variable by grabbing its ID manually
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
}


GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}
GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}


void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
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

Shader::~Shader()
{
	ClearShader();
}
