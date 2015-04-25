#pragma once
#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include "glm\glm\glm.hpp"
#include "glm\glm\gtx\transform.hpp"

using namespace std;
using namespace glm;

char* loadTextFile(const char* file);
GLuint loadShader(const char* file, GLenum shaderType);
GLuint loadShaderProgram(const char* vertexFile, const char* fragmentFile);
void setShaderColor(GLuint programIndex, const char* varName, float red, float green, float blue);
	void setShaderVec2(GLuint programIndex, const char* varName, vec2 vecToSend);
	void setShaderMatrix(GLuint programIndex, const char* name, mat4 value);
