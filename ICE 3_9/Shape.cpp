//includes and usings
#include "Shape.h"
#include <GL\glew.h>
#include <iostream>
#include "ShaderHelpers.h"
#include "glm\glm\glm.hpp"
#include "glm\glm\gtx\transform.hpp"

using namespace std;
using namespace glm;

//global variables
GLuint index;

//constructor for a shape object
Shape::Shape(GLfloat values[], GLsizei numVertices, GLint pIndex)
{
	//sets variable definitions
	programIndex = pIndex;
	vertices = numVertices;
	index = glGetUniformLocation(programIndex,"worldMatrix");
	rot = vec3(0,0,1);
	rotA = 0;

	//creates and binds vertex array and buffer
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vertices*5,values,GL_STATIC_DRAW);

	//creates and enables vertex attrib pointer
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT)*5,0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT)*5,(void*)(sizeof(GL_FLOAT)*2));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//sets offset and scale variables
	offset = glGetUniformLocation(programIndex,"offset");
	scale = glGetUniformLocation(programIndex,"scale");
}

//destructor for a shape object
Shape::~Shape(void)
{
	//destroys vertex array and buffer
	glDeleteVertexArrays(1,&vao);
	glDeleteBuffers(1,&vbo);
}

//draws the shape to the screen
void Shape::Draw(vec3 off, vec3 scl, float ang)
{
	//defines position and scale vectors
	vec3 pos = vec3(off);
	vec3 sca = vec3(scl);

	//defines tranlate, scale, and rotate matrices
	mat4 t = translate(pos);
	mat4 s = glm::scale(sca);
	mat4 r = rotate(ang,rot);

	//defines world matrix
	mat4 world = t * s * r;

	//passes world matrix to shader
	glProgramUniformMatrix4fv(programIndex,index,1,GL_FALSE,&world[0][0]);

	//binds vertex array and draws the object to the screen
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,vertices);
}