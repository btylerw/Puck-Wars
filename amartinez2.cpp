//Aldair Martinez
//new source file
//
#include <iostream>
#include <GL/glx.h>
#include <cmath>
#include "amartinez2.h"

using namespace std;

extern void reset_brick_pos(int xres);


void show_name() {
	printf("aldair\n");
	//return 0;
}


unsigned int manage_state(unsigned int s)
{
	s = s ^ 1;
	return s;
}


extern void help_screen(GLuint help_screen_img, int xres, int yres)
{
	glBindTexture(GL_TEXTURE_2D, help_screen_img);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);   glVertex2i(0,   0);
	glTexCoord2f(0.0f, 0.25f);  glVertex2i(0,   yres);
	glTexCoord2f(1.0f, 0.25f);  glVertex2i(xres, yres);
	glTexCoord2f(1.0f, 1.0f);   glVertex2i(xres, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	/*
	Rect r;
	ggprint16(&r, 0, 0x00ffffff, "Mouse movement: move paddle");
	ggprint16(&r, 0, 0x00ffffff, "F1 Key: close help screen");
	cout << "Mouse movement: move paddle\n";
	cout << "F1 key: close help screen\n";	
	*/
}

//pressing Q key changes paddle shape
extern void draw_circle(float cx, float cy, float radius, int segs)
{
// retrieved from https://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
    glBegin(GL_TRIANGLE_FAN);
    for(int i=0; i<segs; i++) {
	float theta = 3.1415926 * 2 * i / float(segs);
	float x = radius * cosf(theta);
	float y = radius * sinf(theta);
	glVertex2f(x + cx, y + cy);
    }
    glEnd();

}

// p1 is vector, sz is size in pixels
extern void drawPowerUps(float* p1, int sz)
{
	glColor3f(1.0,1.0,0);
	glBegin(GL_QUADS);
	glVertex2i(p1[0]-sz, p1[1]-sz);
	glVertex2i(p1[0]-sz, p1[1]+sz);
	glVertex2i(p1[0]+sz, p1[1]+sz);
	glVertex2i(p1[0]+sz, p1[1]-sz);
	glEnd();
}

//power up
extern float speedUp(float* vel)
{
	vel[0] *= 1.5;
	vel[1] *= 1.5;
	return *vel;
}

extern void reset(float* puckPos, int xres, int yres, 
float* velocity, bool& firstTime, int& brick_feature, 
int& powerUp, int& increaseSZ)
{
	puckPos[0] 		= xres / 2;
    puckPos[1] 		= yres / 2;
    velocity[0] 	= velocity[1] = 0;
    firstTime = true;
	brick_feature = 0;
	powerUp = 0;
	increaseSZ = 1;
	reset_brick_pos(xres);	

}

//limit speed of puck
extern void limit_speed(float* vel, int speedCap)
{
	if (vel[0] > speedCap){
		vel[0] = speedCap;
	}
	if (vel[1] > speedCap){
		vel[1] = speedCap;
	}
}

//places power in random position (within a range)
extern void set_rand_points(float* p1, int xres, int yres)
{
		//x axis
	p1[0] = ((rand() % xres) * 0.75) + (xres * 0.17);
	//y axis
	p1[1] = ((rand() % yres) * 0.7) + (yres * 0.2);
}

//checks distance between puck and power up
extern void check_distance(float* puckPos, float* p1, 
int pUpSize, int& increaseSZ, int& powerUp, float* vel) 
{
	if (abs(puckPos[1] - p1[1]) < pUpSize && 
   	(abs(puckPos[0] - p1[0]) < pUpSize))
	{
		if ((rand() % 2) == 0 && increaseSZ == 1)
		{
			//make paddle bigger
			increaseSZ = 2;	
		}
		else
		{
			//speed up puck
			*vel = speedUp(vel);
		}
		powerUp = 0;  //power up goes away
	}
}
