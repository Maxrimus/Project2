//includes and usings
#include <GL\glew.h>
#include "glfw-3.1.1.bin.WIN32\include\GLFW\glfw3.h"
#include <iostream>
#include <vector>
#include "ShaderHelpers.h"
#include "Shape.h"
#include "Entity.h"
#include "glm\glm\gtx\transform.hpp"
#include <time.h>
#pragma comment(lib, "glfw3.lib")

using namespace std;
using namespace glm;

//global variables
GLint result;//program index
GLfloat ratioXY;//ratio between width and height
double previous;//previous time
double gamePrevious;//previous game time
float red,green,blue;//color values
double lastSpawn;//X spawn location of last rupee
vec3 rotation;//rotation vector
vec3 scaleThis;//scale vector
float rotationAmount;//rotation amount
vec3 position;//position vector
vector<Entity> entities;//vector of entities
vector<vec3> heartLocs;//vector of positions for hearts
int score;//score
bool playing;//whether the game is playing

//shape pointers for all shapes needed
Shape* halfTrapUp;
Shape* EntityS;
Shape* heart;

//definitions for methods
void addEntity();
void drawEntity(float xLoc, float r, float g, float b);

//initializes the program
void init()
{
	cout << "Initializing" << endl;

	//loads program
	result = loadShaderProgram("Shaders/vertexShader.glsl","Shaders/fragmentShader.glsl");

	//initializes global variables
	previous = glfwGetTime()/1000;
	gamePrevious = glfwGetTime();
	position = vec3(0,0,0);
	scaleThis = vec3(1,1,1);
	red = green = blue = 0.0f;
	lastSpawn = rand()%800;
	score = 0;
	heartLocs.push_back(vec3(0.9f,0.9f,0.0f));
	heartLocs.push_back(vec3(0.75f,0.9f,0.0f));
	heartLocs.push_back(vec3(0.6f,0.9f,0.0f));
	playing = true;

	if(result != 0)
	{//uses the program if no error occured
		glUseProgram(result);
	}
	else
	{//reports that an error occured
		cout << "Program error" << endl;
	}
	//defines vertices and creates shapes
	GLfloat values[] = {0.0625f,0.0625f,1.0f,1.0f,1.0f,0.0625f,-0.0625f,1.0f,1.0f,1.0f,-0.0625f,0.0625f,1.0f,1.0f,1.0f,
		-0.0625f,0.0625f,1.0f,1.0f,1.0f,-0.0625f,-0.0625f,1.0f,1.0f,1.0f,0.0625f,-0.0625f,1.0f,1.0f,1.0f,0.0625f,0.0625f,1.0f,1.0f,1.0f,
		-0.0625f,0.0625f,1.0f,1.0f,1.0f,0.0f,0.14f,1.0f,1.0f,1.0f,0.0625f,-0.0625f,1.0f,1.0f,1.0f,-0.0625f,-0.0625f,1.0f,1.0f,1.0f,0.0f,
		-0.14f,1.0f,1.0f,1.0f};
	EntityS = new Shape(values,12,result);
	GLfloat values3[] = {0.5f,0.5f,1.0f,1.0f,1.0f,0.5f,-0.5f,1.0f,1.0f,1.0f,-0.5f,0.5f,1.0f,1.0f,1.0f,
		-0.5f,-0.5f,1.0f,1.0f,1.0f,-0.5f,0.5f,1.0f,1.0f,1.0f,0.5f,-0.5f,1.0f,1.0f,1.0f,0.5f,0.5f,1.0f,1.0f,1.0f,
		0.5f,-0.5f,1.0f,1.0f,1.0f,0.75f,-0.5f,1.0f,1.0f,1.0f,-0.5f,0.5f,1.0f,1.0f,1.0f,-0.5f,-0.5f,1.0f,1.0f,1.0f,
		-0.75f,-0.5f,1.0f,1.0f,1.0f};
	halfTrapUp = new Shape(values3,12,result);
	GLfloat values1[] = {0.0f,-0.5f,1.0f,1.0f,1.0f,-0.5f,0.0f,1.0f,1.0f,1.0f,0.5f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f,
		0.5f,0.0f,1.0f,1.0f,1.0f,0.5f,0.125f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.125f,1.0f,1.0f,1.0f,0.5f,0.125f,1.0f,1.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,1.0f,-0.5f,0.0f,1.0f,1.0f,1.0f,-0.5f,0.125f,1.0f,1.0f,1.0f,0.0f,0.125f,1.0f,1.0f,1.0f,-0.5f,0.125f,1.0f,1.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,1.0f,-0.5f,0.125,1.0f,1.0f,1.0f,0.0f,0.125f,1.0f,1.0f,1.0f,-0.25f,0.25f,1.0f,1.0f,1.0f,0.0f,0.125f,1.0f,1.0f,1.0f,
		0.5f,0.125f,1.0f,1.0f,1.0f,0.25f,0.25f,1.0f,1.0f,1.0f};
	heart = new Shape(values1,21,result);

	//sets background color
	glClearColor(0.392f,0.807f,0.921f,1.0f);
}

//called every frame
void update()
{
	//gets dt
	double currentTime = glfwGetTime()/1000;
	double dt = currentTime - previous;
	previous = currentTime;

	for(int i = 0; i < (int)(entities.size());i++)
	{//updates all entities
		entities[i].update(dt);
	}
}

//draws objects to the screen
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//draws the basket
	setShaderColor(result,"color",0.36f,0.20f,0.09f);
	halfTrapUp->Draw(vec3(0.0f,-0.7f,0.0f) + position,vec3(0.2f,-0.2f,1),0);

	//draws hearts
	setShaderColor(result,"color",1.0f,0.0f,0.0f);
	for(unsigned int i = 0; i < heartLocs.size(); i++)
	{
		heart->Draw(heartLocs[i],vec3(0.85f * ratioXY * 0.15f,0.15f,1.0f),0);
	}

	if(playing)
	{//game loop
		addEntity(); //adds a new entity
		for(int i = 0; i < (int)(entities.size());i++)
		{//loops through all entities
			entities[i].draw();//draws entity
			if(entities[i].position.y < -.6 && entities[i].position.y > -.8)
			{//determines if entity is within basket
				if(entities[i].position.x < position.x + .1 && entities[i].position.x > position.x - .1)
				{
					switch(entities[i].rarity)
					{//adds to score based on rarity
						case 1:
							score += 1;
							break;
						case 2:
							score += 5;
							break;
						case 3:
							score += 20;
							break;
					}
					//erases entity from vector
					entities.erase(entities.begin() + i);
					cout << score << endl;
					/*
					int mult = score / 2000;
					heartLocs[2+mult] = vec3(heartLocs[2].x - .15*mult,0.9f,0.0f);
					*/
				}
			}
			else if(entities[i].position.y <= -1)
			{//destroys entity and removes a heart if entity is off bottom of screen
				entities.erase(entities.begin() + i);
				heartLocs.pop_back();
				if(heartLocs.size() == 0)
				{
					playing = false;
				}
			}
		}
	}
	else
	{
		//print game over and score
		cout << "Game Over" << endl;
		cout << "Score: " << score << endl;
	}
	//draws all queued objects
	glFlush();
}

//sets position.x to cursor position.x
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	position.x = (float)((2 * xpos)/(800) - 1);
}

//adds an entity to the vector
void addEntity()
{
	//seeds random
	//srand(unsigned int(time(NULL)));

	//gets dt
	double currentTime = glfwGetTime();
	double dt = currentTime - gamePrevious;
	
	//variables for later use
	int dev = 100;
	int rar = 1;
	if(dt > 1)
	{//creates a new entity every second
		//determines rarity
		int EntityRar = rand()%100;
		//determines x location
		double x = (2 * lastSpawn)/(800) - 1;

		//if statements determine color of entity
		if(EntityRar >= 0 && EntityRar < 60)
		{
			red = blue = 0.0f;
			green = 1.0f;
			rar = 1;
		}
		else if(EntityRar >= 60 && EntityRar < 90)
		{
			red = green = 0.0f;
			blue = 1.0f;
			rar = 2;
		}
		else if(EntityRar >= 90 && EntityRar < 100)
		{
			green = blue = 0.0f;
			red = 1.0f;
			rar = 3;
		}

		//determines percentage and direction for new spawn
		int perc = rand()%100;
		int dir = rand()%2;

		//if statements to determine location based on lastSpawn
		if(perc >= 0 && perc < 60)
		{
			switch(dir)
			{
				case 0:
				lastSpawn -= dev;
					break;
				case 1:
				lastSpawn += dev;
					break;
			}
		}
		else if(perc >= 60 && perc < 95)
		{
			switch(dir)
			{
				case 0:
				lastSpawn -= 2*dev;
					break;
				case 1:
				lastSpawn += 2*dev;
					break;
			}
		}
		else if(perc >= 95 && perc < 99)
		{
			switch(dir)
			{
				case 0:
				lastSpawn -= 3*dev;
					break;
				case 1:
				lastSpawn += 3*dev;
					break;
			}
		}
		else
		{
			switch(dir)
			{
				case 0:
				lastSpawn -= 4*dev;
					break;
				case 1:
				lastSpawn += 4*dev;
					break;
			}
		}

		//sets spawns to edge of screen if off screen
		if(lastSpawn < 50)
		{
			lastSpawn = 100;
		}
		if(lastSpawn > 750)
		{
			lastSpawn = 700;
		}

		//determines x velocity
		int velMult = rand()%20;
		int posMult = rand()%2;
		float velX = 0;
		switch(posMult)
		{
			case 0:
			velX = 0.001f * velMult;
				break;
			case 1:
			velX = -0.001f * velMult;
				break;
		}

		//determines radial velocity
		int wMult = rand()%5;
		int wposMult = rand()%2;
		double w = 0;
		switch(wposMult)
		{
			case 0:
			w = 0.1 * wMult;
				break;
			case 1:
			w = -0.1 * wMult;
				break;
		}

		//adds new entity to vector
		entities.push_back(Entity(x,1.0,red,green,blue,w,0,result,EntityS,rar,velX));

		//sets previous time to current time
		gamePrevious = currentTime;
	}
}

//main method of the program
int main(int argc, char** argv)
{
	//glfw black magic setup
	GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

	//sets up openGL window
	glewExperimental=GL_TRUE;

	//determines if program is working
	if(glewInit() != GLEW_OK) {return -1;}

	//calls init and defines ratio of the width to the height of the screen
	init();
	ratioXY = 600.0/800.0;

    while (!glfwWindowShouldClose(window))
    {//main loop
        glfwSwapBuffers(window);
        glfwPollEvents();
		glfwSetCursorPosCallback(window,cursor_position_callback);
		update();
		draw();
    }

    glfwTerminate();
    return 0;
}