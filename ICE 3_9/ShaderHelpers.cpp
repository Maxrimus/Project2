#include "ShaderHelpers.h"
#include <iostream>
#include <fstream>
#include <string>
#include "glm\glm\glm.hpp"
#include "glm\glm\gtx\transform.hpp"

using namespace std;
using namespace glm;

//All purpose function to load a text file
char* loadTextFile(const char* file)
{
	//opens a stream to the file in binary mode
	ifstream inFile(file, ios::binary);

	if(inFile.is_open())
	{//determines if file is open so it can be edited
		//determines length of file
		inFile.seekg(0, ios::end);
		int length = (int)inFile.tellg();

		//goes back to beginning of file
		inFile.seekg(0, ios::beg);

		//creates a char array
		char* fileContents = new char[length + 1];

		//puts files contents into char array
		inFile.read(fileContents, length);
		fileContents[length] = 0;

		//returns file contents
		return fileContents;
	}
	else
	{//returns 0 if an error occured
		return 0;
	}
}

//Uses file loading function to load the shader programs from text files
GLuint loadShader(const char* file, GLenum shaderType)
{
	//gets files contents
	const char* fileContents = loadTextFile(file);
	if(fileContents == 0)
	{//determines if an error occured in loading a file
		cout << "File not loaded" << endl;
		return 0;
	}

	//creates shader program and compiles it
	GLuint shaderIndex = glCreateShader(shaderType);
	glShaderSource(shaderIndex,1,&fileContents,0);
	glCompileShader(shaderIndex);

	delete[] fileContents;

	//gets status of compile
	GLint index = 0;
	glGetShaderiv(shaderIndex,GL_COMPILE_STATUS,&index);

	if(index == 0)
	{//compile failed, prints error to console
		cout << "Shader compile error" << endl;
		GLint length;

		//gets full log
		glGetShaderiv(shaderIndex,GL_INFO_LOG_LENGTH,&length);
		char* log = new char[length + 1];
		glGetShaderInfoLog(shaderIndex,length,0,log);

		//prints log
		cout << log << endl;

		//deletes shader and returns 0 to denote an error has occured
		glDeleteShader(shaderIndex);
		delete[] log;
		return 0;
	}
	else
	{//no error has occured, returns index of the shader program
		return shaderIndex;
	}
}

//uses the shader loading function to load and compile the vertex and fragment shader programs
GLuint loadShaderProgram(const char* vertexFile, const char* fragmentFile)
{
	//loads vertex shader
	GLuint result1 = loadShader(vertexFile,GL_VERTEX_SHADER);

	if(result1 == 0)
	{//determines if an error has occured while loading the vertex file
		cout << "Vertex File error" << endl;
		return 0;
	}

	//loads fragment shader
	GLuint result2 = loadShader(fragmentFile, GL_FRAGMENT_SHADER);

	if(result2 == 0)
	{//determines if an error has occured while loading the fragment file
		cout << "Fragment File error" << endl;
		return 0;
	}

	//creates a shader program and attaches the vertex and fragment shaders to it
	GLuint programIndex = glCreateProgram();

	glAttachShader(programIndex,result1);
	glAttachShader(programIndex,result2);

	//links the program and checks it's status
	glLinkProgram(programIndex);

	GLint linked;
	glGetProgramiv(programIndex,GL_LINK_STATUS,&linked);

	if(linked == 0)
	{//determines if an error occured while creating the program
		cout << "Shader linking error" << endl;
		GLint length;
		//gets log contents
		glGetProgramiv(programIndex,GL_INFO_LOG_LENGTH,&length);
		char* log = new char[length + 1];
		glGetProgramInfoLog(programIndex,length,0,log);

		//prints log
		cout << log << endl;

		//deletes program and returns a 0 to indicate an error has occured when creating the program
		glDeleteProgram(programIndex);
		delete[] log;
		return 0;
	}
	else
	{//if no errors occured returns the program index
		return programIndex;
	}
}

//sets the color variable in the fragment shader to a specific color
void setShaderColor(GLuint programIndex, const char* varName, float red, float green, float blue)
{
	GLuint index = glGetUniformLocation(programIndex,varName);
	glProgramUniform4f(programIndex,index,red,green,blue,1.0f);
}

void setShaderVec2(GLuint programIndex, const char* varName, vec2 vecToSend)
{
	GLuint index = glGetUniformLocation(programIndex,varName);
	glProgramUniform2fv(programIndex,index,1,&vecToSend[0]);
}