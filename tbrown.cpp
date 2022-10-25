// Tyler Brown
// Source file
#include <GL/glx.h>
#include "tbrown.h"
#include <iostream>
#include <cstdlib>

// Renders intro screen
extern void show_intro_screen(GLuint introTexture, int xres, int yres)
{
        glBindTexture(GL_TEXTURE_2D, introTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f);   glVertex2i(0,   0);
            glTexCoord2f(0.0f, 0.25f);  glVertex2i(0,   yres);
            glTexCoord2f(1.0f, 0.25f);  glVertex2i(xres, yres);
            glTexCoord2f(1.0f, 1.0f);   glVertex2i(xres, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
}

extern void show_background(GLuint backgroundTexture, int xres, int yres)
{
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f);   glVertex2i(0,   0);
            glTexCoord2f(0.0f, 0.25f);  glVertex2i(0,   yres);
            glTexCoord2f(1.0f, 0.25f);  glVertex2i(xres, yres);
            glTexCoord2f(1.0f, 1.0f);   glVertex2i(xres, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
}

class Bricks {
	public:
		float w;
		float h;
		float x;
		float y;
		float pos[2];
		float vel[2];
		Bricks() {
			h = 12.0f;
			w = 60.0f;
			pos[0] = x = 0.0;
			pos[1] = y = 0.0;
			vel[0] = vel[1] = 0;
		}
};

// Creates bricks to obstruct puck
Bricks bricks[20];

// Picks random brick to move
// If brick is on left side of screen, moves right
// If brick is on right side of screen, moves left
extern int change_brick_vel(int i, float pos)
{
	srand(time(NULL));
	int selection = rand() % 19 + 1;

	if (i < 10 && i == selection) {
		return 10;
	}
	else if (i >= 10 && i == selection) {
		return -10;
	}
	else
		return 0;
}

// Default everything
extern void reset_brick_pos(int xres)
{
	for (int i = 0; i < 20; i++) {
    	if (i < 10) {
			bricks[i].pos[0] = -bricks[i].w;
    	}

    	else
			bricks[i].pos[0] = bricks[i].w + xres;
	}
}

// Will move bricks to appropriate spots when program starts
extern void make_bricks(int x, int y)
{
	int yres = y;
	for (int i = 0; i < 20; i++) {
		if (i < 10) {
			bricks[i].pos[0] = -bricks[i].w;
		}
		else if (i == 10) {
			yres = y - 20;
			bricks[i].pos[0] = x + bricks[i].w;
		}
		else {
			bricks[i].pos[0] = x + bricks[i].w;
		}
		bricks[i].pos[1] = yres;
		yres-=100;
	}
}

// Check for collision with puck. Needs more work to check underside collision
extern void check_brick_hit(float &puckw, float &puckpos0, float &puckpos1, float &puckvel0, float &puckvel1)
{
	for (int i = 0; i < 20; i++) {
		if ((puckpos1 - puckw) < (bricks[i].pos[1] + bricks[i].h) &&
		puckpos1 > (bricks[i].pos[1] - bricks[i].h) &&
        puckpos0 > (bricks[i].pos[0] - bricks[i].w) &&
        puckpos0 < (bricks[i].pos[0] + bricks[i].w)) {
        	if (puckpos0 > bricks[i].pos[0]) {
 	       		puckvel1 = 0;
           		puckpos1 = bricks[i].pos[1] + bricks[i].h;
           		puckvel1 += bricks[i].vel[1];
           		puckvel0 = 2;
           		if (bricks[i].vel[1] <= 0)
               		puckvel1 = 0;
        	}

        	else if (puckpos0 < bricks[i].pos[0]) {
                puckvel1 = 0;
                puckpos1 = bricks[i].pos[1] + bricks[i].h;
                puckvel1 += bricks[i].vel[1];
                puckvel0 = -2;
                if (bricks[i].vel[1] <= 0)
                    puckvel1 = 0;
       		}

       		else {
            	puckvel1 = 0;
                puckpos1 = bricks[i].pos[1] + bricks[i].h;
                puckvel1 += bricks[i].vel[1];
                if (bricks[i].vel[1] <= 0) {
                    float new_vel = -puckvel1 / 3;
                    puckvel1 = 0;
                    puckvel1 = new_vel;
                }
            }

		}
	}
}

// Renders bricks on screen
extern void draw_bricks()
{
	for (int i = 0; i < 20; i++) {
		glPushMatrix();
		glColor3ub(0, 0, 0);
		glTranslatef(bricks[i].pos[0], bricks[i].pos[1], 0.0f);
		glBegin(GL_QUADS);
			glVertex2f(-bricks[i].w, -bricks[i].h);
			glVertex2f(-bricks[i].w, bricks[i].h);
			glVertex2f(bricks[i].w, bricks[i].h);
			glVertex2f(bricks[i].w, -bricks[i].h);
		glEnd();
		glPopMatrix();
	}	
}

// Updates brick positions, will probably be combined with change_brick_vel() in future
extern void move_bricks()
{
	for (int i = 0; i < 20; i++) {
		bricks[i].pos[0] += change_brick_vel(i, bricks[i].pos[0]);
		if (bricks[i].pos[0] > (600 + bricks[i].w) || bricks[i].pos[0] < -bricks[i].w) {
			if (i < 10)
				bricks[i].pos[0] = -bricks[i].w;
			else
				bricks[i].pos[0] = 600 + bricks[i].w;
		}
	}
}
