//Author: Anh Vu
//Date: 10/4/2022
//source file for avu2.h
//
#include <X11/keysym.h>
#include <GL/glx.h>
#include "avu2.h"
#include <math.h>
#include <iostream>
#include <cstdlib>

//std::string credit_html =
//#include "credit.html";

/*unsigned int manage_state(unsigned int s)
  {
  s = s ^ 1;
  return s;
  }*/

extern int showCredit(GLuint creditTexture, int xres, int yres) 
{	  
    glBindTexture(GL_TEXTURE_2D, creditTexture);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);   glVertex2i(0,    0);
    glTexCoord2f(0.0f, 0.25f);  glVertex2i(0,    yres);
    glTexCoord2f(1.0f, 0.25f);  glVertex2i(xres, yres);
    glTexCoord2f(1.0f, 1.0f);   glVertex2i(xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    return 0;
}

extern void showFeature(int xres, int yres) {
    //Draw a border using a triangle strip
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Blend function show both contain      
    glEnable(GL_BLEND);
    glColor3f(1.0, 1.0, 0.0);
    glColor4f(1.0, 1.0, 0.0, 0.5);
    //last number is blend %
    int w = 40;
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2i(0,   0);
    glVertex2i(0+w, w);
    glVertex2i(0,   yres);
    glVertex2i(0+w, yres-w);
    glVertex2i(xres,   yres);
    glVertex2i(xres-w, yres-w);
    glVertex2i(xres,   0);
    glVertex2i(xres-w, w);
    /*glVertex2i(0,   0);
      glVertex2i(0+w, w);*/
    glEnd();
    glDisable(GL_BLEND);
}

//cheat code box will be displayed here
extern void drawCheatCode(int xres, int yres) {
    //Draw a border using a triangle strip
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Blend function show both contain      
    glEnable(GL_BLEND);
    glColor3f(1.0, 1.0, 0.0);
    glColor4f(2.0, 1.0, 0.0, 0.5);
    int w = 100;
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2i(0+w   , yres-w);
    glVertex2i(0+w   , w);
    glVertex2i(xres-w, w);
    glVertex2i(xres-w, yres-w);
    glVertex2i(0+w, yres-w);
    glVertex2i(0+w, w);
    glEnd();
    glDisable(GL_BLEND);
}

//passcode
void drawCodeBox(int xres, int yres) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor3f(1.0, 1.0, 0.0);
    glColor4f(4.0, 4.0, 4.0, 0.5);
    int w = 400;
    glBegin(GL_TRIANGLE_STRIP);
    //glVertex2i(0+w   , yres-w);
    glVertex2i(0+w   , w);
    glVertex2i(xres-w, w);
    //glVertex2i(xres-w, yres-w);
    glEnd();
    glDisable(GL_BLEND);

}
//////////////////////////////////////////
void drawCircle(int x, int y, GLfloat xcenter, GLfloat ycenter) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle
    GLfloat twicePi = 2.0f * 3.14;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
	glVertex2f(
		x + ((xcenter+1)* cos(i * twicePi / triangleAmount)),
		y + ((ycenter-1)* sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}
//function call
void showCheat(int xres, int yres) {
    float r = 4;
    int b;
    srand((unsigned)time(NULL));
    b = rand() % 10;
    drawCheatCode(xres, yres);
    drawCodeBox(xres, yres);
    for (int i = 0; i < 10; i++) {
    	drawCircle(xres/2, yres/2 + 5*b*i, 30, 30);
    }
}


