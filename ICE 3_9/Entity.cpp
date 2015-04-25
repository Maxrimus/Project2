//includes and usings
#include "Entity.h"
#include "Shape.h"
#include "ShaderHelpers.h"
#include "glm\glm\glm.hpp"
#include "glm\glm\gtx\transform.hpp"
#include <GL\glew.h>
#include "glfw-3.1.1.bin.WIN32\include\GLFW\glfw3.h"
#include <iostream>

using namespace std;
using namespace glm;

//global variables
double ratioXY = 600.0/800.0;
vec3 acc;

//constructor
Entity::Entity(double locX, double locY, float r, float g, float b, double w, int ang, GLuint res, Shape* entity, int rar, float velX)
{
	//defines variables based on arguments passed in
	entityS = entity;
	position = vec3(locX, locY, 0);
	scaleThis = vec3(0.9f,0.9f,0.9f);
	red = r;
	blue = b;
	green = g;
	omega = w;
	angle = (float)(ang);
	result = res;
	rarity = rar;
	this->velocity = vec3(velX,-0.003f * rar,0.0f);
	acc = vec3(0.0f,-0.001f,0.0f);
}

//destructor
Entity::~Entity(void)
{
}

//draws entity
void Entity::draw()
{
	setShaderColor(result,"color",0.0f,0.0f,0.0f);
	entityS->Draw(position,vec3(1.0f*ratioXY*1.3,1.0f*1.3,0.0f) * scaleThis,angle);
	setShaderColor(result,"color",red,green,blue);
	entityS->Draw(position,vec3(1.0f*ratioXY,1.0f,0.0f) * scaleThis,angle);
}

//updates entity
void Entity::update(double dt)
{
	//updates angle, velocity, and position
	angle += float(omega);
	velocity += acc;
	position += velocity;

	//"bounces" entity off edge of screen
	if(position.x >= (1-(0.0625/2)))
	{
		velocity.x *= -1;
	}

	if(position.x <= -(1-(0.0625/2)))
	{
		velocity.x *= -1;
	}
}