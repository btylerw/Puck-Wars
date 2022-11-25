// Tyler Brown
// Source file
#include <GL/glx.h>
#include "tbrown.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
// Renders intro screen

int difficulty = 0;
int autoplay = 0;

extern void set_difficulty(int a)
{
	difficulty = a;
}

extern int check_autoplay()
{
	return autoplay;
}

extern void set_autoplay(int a)
{
	autoplay = a;
}
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
		float old_x;
		float pos[2];
		float vel[2];
		float ai_return;
		float auto_return;
		float old_y;
		float force;
		Bricks() {
			h = 12.0f;
			w = 60.0f;
			pos[0] = x = 0.0;
			pos[1] = y = 0.0;
			vel[0] = vel[1] = 0;
			old_x = 0;
			old_y = 0;
			force = 0;
			ai_return = pos[1];
			auto_return = pos[1];
		}
};

// Creates bricks to obstruct puck
Bricks bricks[20];
Bricks goals[2];
Bricks ai_paddle;
Bricks autoplay_paddle;

extern void set_goals(int x, int y)
{
   	goals[0].h = 12.0f;
	goals[0].w = 97.5f;
	goals[1].h = 12.0f;
	goals[1].w = 97.5f;
	goals[0].pos[0] = x / 2;
	goals[0].pos[1] = goals[0].h;
	goals[1].pos[0] = x / 2;
	goals[1].pos[1] = y - goals[0].h;
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
	ai_paddle.w = 30.0f;
	ai_paddle.pos[0] = x/2.0;
	ai_paddle.pos[1] = y/1.1;
	ai_paddle.old_x = x/2.0;
	ai_paddle.old_y = y/1.1;
	ai_paddle.ai_return = ai_paddle.pos[1];
	autoplay_paddle.w = 30.0f;
	autoplay_paddle.pos[0] = x/2.0;
	autoplay_paddle.pos[1] = y/10.0;
	autoplay_paddle.old_x = x/2.0;
	autoplay_paddle.old_y = y/10.0;
	autoplay_paddle.auto_return = autoplay_paddle.pos[1];
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
				puckvel1 = -puckvel1;
        	}

        	else if (puckpos0 < bricks[i].pos[0]) {
                puckvel1 = -puckvel1;
       		}

       		else {
            	puckvel1 = -puckvel1;
            }

		}
	}
}

// Renders bricks on screen
extern void draw_bricks()
{
	for (int i = 0; i < 20; i++) {
		// Fill obstacle color
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
		// Make line border for obstacles
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glTranslatef(bricks[i].pos[0], bricks[i].pos[1], 0.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(-bricks[i].w, -bricks[i].h);
			glVertex2f(-bricks[i].w, bricks[i].h);
			glVertex2f(bricks[i].w, bricks[i].h);
			glVertex2f(bricks[i].w, -bricks[i].h);
		glEnd();
		glPopMatrix();
	}
	// Uncomment to check goal size
	/*
	for (int i = 0; i < 2; i++) {
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glTranslatef(goals[i].pos[0], goals[i].pos[1], 0.0f);
		glBegin(GL_QUADS);
			glVertex2f(-goals[i].w, -goals[i].h);
			glVertex2f(-goals[i].w, goals[i].h);
			glVertex2f(goals[i].w, goals[i].h);
			glVertex2f(goals[i].w, -goals[i].h);
		glEnd();
		glPopMatrix();
	}
	*/
    glPushMatrix();
    glColor3ub(100, 200, 100);

	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 360; i++) {
		float theta = 3.1415926 * 2 * i / 360.0;
		float x = 30.0 * cosf(theta);
		float y = 30.0 * sinf(theta);
		glVertex2f(x + ai_paddle.pos[0], y + ai_paddle.pos[1]);
	}
	glEnd();

	if (autoplay) {
		glColor3ub(100, 100, 200);
		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i < 360; i++) {
			float theta = 3.1415926 * 2 * i / 360.0;
			float x = 30.0 * cosf(theta);
			float y = 30.0 * sinf(theta);
			glVertex2f(x + autoplay_paddle.pos[0], y + autoplay_paddle.pos[1]);
		}
		glEnd();
	}
}

// Updates brick positions, will probably be combined with change_brick_vel() in future
extern void move_bricks()
{	
	srand(time(NULL));
	for (int i = 0; i < 20; i++) {
		int selection = rand() % 19 + 1;

		if (i < 10 && i == selection) {
			bricks[i].pos[0]+=10;
		}
		else if (i >= 10 && i == selection) {
			bricks[i].pos[0]+=-10;
		}
		//bricks[i].pos[0] += change_brick_vel(i, bricks[i].pos[0]);
		if (bricks[i].pos[0] > (600 + bricks[i].w) || bricks[i].pos[0] < -bricks[i].w) {
			if (i < 10)
				bricks[i].pos[0] = -bricks[i].w;
			else
				bricks[i].pos[0] = 600 + bricks[i].w;
		}
	}
}

// Checks if puck goes into AI goal
extern int check_player_goal(float puckpos0, float puckpos1, float puckw)
{
        if ((puckpos1 - puckw) < (goals[1].pos[1] + goals[1].h) &&
        puckpos1 > (goals[1].pos[1] - goals[1].h) &&
        puckpos0 > (goals[1].pos[0] - goals[1].w) &&
        puckpos0 < (goals[1].pos[0] + goals[1].w)) {
			return 1;
		}
		else
			return 0;
	
}

// Checks if puck goes into your goal
extern int check_ai_goal(float puckpos0, float puckpos1, float puckw)
{
	if ((puckpos1 + puckw) > (goals[0].pos[1] - goals[0].h) &&
	puckpos1 < (goals[0].pos[1] + goals[0].h) &&
	puckpos0 > (goals[0].pos[0] - goals[0].w) &&
	puckpos0 < (goals[0].pos[0] + goals[0].w)) {
		return 1;
	}
	else
		return 0;
}

// AI paddle tracks puck location, moves back to center if not coming towards it's side
extern void ai_paddle_physics(float puckpos0, float puckpos1, float puckw, float &puckvel1, float &puckvel0, int y)
{
	int speed = 0;
	switch (difficulty) {
		case 0:	speed = 2;
				break;
		case 1: speed = 5;
				break;
		case 2: speed = 8;
				break;
	}
	ai_paddle.pos[0] += ai_paddle.vel[0];
	ai_paddle.pos[1] += ai_paddle.vel[1];
	if ((puckpos1 - puckw) < (ai_paddle.pos[1] + ai_paddle.h) &&
        puckpos1 > (ai_paddle.pos[1] - ai_paddle.h) &&
        puckpos0 > (ai_paddle.pos[0] - ai_paddle.w) &&
        puckpos0 < (ai_paddle.pos[0] + ai_paddle.w)) {
			if (puckpos0 < ai_paddle.pos[0])
				puckvel0 = -2;
			else if (puckpos0 > ai_paddle.pos[0])
				puckvel0 = 2;
			puckvel1 = -puckvel1 - 5;
		}

	if (puckpos1 > y / 2 && puckvel1 > 0) {
		if (puckpos0 > ai_paddle.pos[0] + ai_paddle.w) {
			ai_paddle.vel[0] = speed;
		}
		else if (puckpos0 < ai_paddle.pos[0] - ai_paddle.w) {
			ai_paddle.vel[0] = -speed;
		}
		if (puckpos1 < ai_paddle.pos[1]) {
			ai_paddle.vel[1] = -speed;
		}
	}
	else if (ai_paddle.pos[1] < ai_paddle.ai_return) {
		ai_paddle.vel[1] = speed;
		ai_paddle.vel[0] = 0;
	}
	else {
		ai_paddle.vel[0] = 0;
		ai_paddle.vel[1] = 0;
	}

	if (autoplay) {

		autoplay_paddle.pos[0] += autoplay_paddle.vel[0];
		autoplay_paddle.pos[1] += autoplay_paddle.vel[1];
		if ((puckpos1 - puckw) < (autoplay_paddle.pos[1] + autoplay_paddle.h) &&
        	puckpos1 > (autoplay_paddle.pos[1] - autoplay_paddle.h) &&
        	puckpos0 > (autoplay_paddle.pos[0] - autoplay_paddle.w) &&
        	puckpos0 < (autoplay_paddle.pos[0] + autoplay_paddle.w)) {
				if (puckpos0 < autoplay_paddle.pos[0])
					puckvel0 = -2;
				else if (puckpos0 > autoplay_paddle.pos[0])
					puckvel0 = 2;
				puckvel1 = -puckvel1 + 5;
			}

		if (puckpos1 < y / 2 && puckvel1 < 0) {
			if (puckpos0 > autoplay_paddle.pos[0] + autoplay_paddle.w) {
				autoplay_paddle.vel[0] = speed;
			}
			else if (puckpos0 < autoplay_paddle.pos[0] - autoplay_paddle.w) {
				autoplay_paddle.vel[0] = -speed;
			}
			if (puckpos1 > autoplay_paddle.pos[1]) {
				autoplay_paddle.vel[1] = speed;
			}
		}
		else if (autoplay_paddle.pos[1] > autoplay_paddle.auto_return) {
			autoplay_paddle.vel[1] = -speed;
			autoplay_paddle.vel[0] = 0;
		}
		else {
			autoplay_paddle.vel[0] = 0;
			autoplay_paddle.vel[1] = 0;
		}
	}
}
