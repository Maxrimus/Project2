#pragma once
#include "Shape.h"
class Entity
{
public:
	Entity(double locX, double locY, float r, float g, float b, double w, int ang, GLuint res, Shape* entity, int rar, float velX);
	~Entity(void);
	void draw();
	void update(double dt);
	vec3 position;
	int rarity;
private:
	vec3 scaleThis;
	float red, green, blue;
	double omega;
	float angle;
	Shape* entityS;
	GLuint result;
	vec3 velocity;
};

