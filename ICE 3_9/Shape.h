#pragma once
#include <GL\glew.h>
#include <iostream>
#include "ShaderHelpers.h"

using namespace std;
class Shape
{
public:
	Shape(GLfloat values[], GLsizei numVertices, GLint pIndex);
	~Shape(void);
	void Shape::Draw(vec3 off, vec3 scl, float ang);
	vec3 rot;
	float rotA;
private:
	GLuint vbo;
	GLuint vao;
	GLsizei vertices;
	GLint programIndex;
	GLint offset;
	GLint scale;
};

