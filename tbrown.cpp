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

extern int reset_brick_pos(int i, float w, int xres)
{
    if (i < 10) {
		return -w;
    }

    else
		return w + xres;
}
