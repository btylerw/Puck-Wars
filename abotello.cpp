// Andres Botello
// sourcefile
#include <GL/glx.h>
#include "abotello.h"
#include <iostream>
//pause state
unsigned int manage_pstate(unsigned int s)
{
    s = s ^ 1;
    return s;
}
unsigned int manage_cstate(unsigned int j)
{
    j = j ^ 1;
    return j;
}
unsigned int manage_sstate(unsigned int l)
{
    l = l ^ 1;
    return l;
}
unsigned int manage_bstate(unsigned int b)
{
    b = b ^ 1;
    return b;
}
extern void pause_screen(GLuint pauseTexture, int xres, int yres)
{
    glBindTexture(GL_TEXTURE_2D, pauseTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);   glVertex2i(0,   0);
    glTexCoord2f(0.0f, 0.25f);  glVertex2i(0,   yres);
    glTexCoord2f(1.0f, 0.25f);  glVertex2i(xres, yres);
    glTexCoord2f(1.0f, 1.0f);   glVertex2i(xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}
