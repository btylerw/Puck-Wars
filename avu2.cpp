//Author: Anh Vu
//Date: 10/4/2022
//source file for avu2.h
//
#include <X11/keysym.h>
#include <GL/glx.h>
#include "avu2.h"
#include <math.h>
#include <cstdlib>
#include <GL/glut.h>
#include "fonts.h"

//Interact with mouse check function on main
extern void printNumber(int xres, int yres, int num, int enter) {
    Rect r;
    r.bot = yres/2 + 73;
    if (enter == 0) {
        r.left = xres/2 - 81;
        if (num == 1) {
            ggprint16(&r, 0, 0x0088aaff, "1");
        }
        if (num == 2) {
            ggprint16(&r, 0, 0x0088aaff, "2");
        }
        if (num == 3) {
            ggprint16(&r, 0, 0x0088aaff, "3");
        }
    }
    else if (enter == 1) {
        r.left = xres/2;
        if (num == 1) {
            ggprint16(&r, 0, 0x0088aaff, "1");
        }
        if (num == 2) {
            ggprint16(&r, 0, 0x0088aaff, "2");
        }
        if (num == 3) {
            ggprint16(&r, 0, 0x0088aaff, "3");
        }
    }
    else if (enter == 2) {
        r.left = xres/2 + 80;
        if (num == 1) {
            ggprint16(&r, 0, 0x0088aaff, "1");
        }
        if (num == 2) {
            ggprint16(&r, 0, 0x0088aaff, "2");
        }
        if (num == 3) {
            ggprint16(&r, 0, 0x0088aaff, "3");
        }
    }
}
//Credit screen
extern int showCredit(GLuint creditTexture, int xres, int yres) 
{	
    glBindTexture(GL_TEXTURE_2D, creditTexture);
    Rect r;  
    //glColor3f(0.2, 0.2, 0.2);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);   glVertex2i(0,    0);
    glTexCoord2f(0.0f, 0.25f);  glVertex2i(0,    yres);
    glTexCoord2f(1.0f, 0.25f);  glVertex2i(xres, yres);
    glTexCoord2f(1.0f, 1.0f);   glVertex2i(xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    r.left = xres/2;
    r.bot = yres - 90;
    ggprint16(&r, 0, 0x0000CCCC, "Team 7");
    r.bot = yres - 140;
    ggprint16(&r, 0, 0x0000CCCC, "Tyler, Andres, Aldair, Anh, Abisai");
    r.bot = yres - 210;
    ggprint16(&r, 0, 0x0000CCCC, "PUCK WARS - INSPIRED BY AIR HOCKEY");
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
    glEnd();
    glDisable(GL_BLEND);
}

//Place for cheat code
extern void drawCheatCode(int xres, int yres) {
    //Draw a border using a triangle strip
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Blend function show both contain      
    glEnable(GL_BLEND);
    glColor3f(1.0, 1.0, 0.0);
    glColor4f(2.0, 1.0, 0.0, 0.5);
    int w = 150;
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2i(0+w   , yres-2*w);
    glVertex2i(0+w   , w*2);
    glVertex2i(xres-w, w*2);
    glVertex2i(xres-w, yres-2*w);
    glVertex2i(0+w, yres-2*w);
    glVertex2i(0+w, w*2);
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

//Circles
void greenCircle(int xres, int yres) {
    Rect r;
    r.bot = xres/2;
    r.left = yres/2;
    r.center = 0;
    //green
    glColor3f(0.0f, 1.0f, 0.0f);
    drawCircle(xres/2 - 80, yres/2 + 80, 30, 30);
    drawCircle(xres/2 + 80, yres/2 + 80, 30, 30);
    drawCircle(xres/2     , yres/2 + 80, 30, 30);
    //cyan
    glColor3f(0.5f, 1.0f, 1.0f);
    drawCircle(xres/2     , yres/2 - 20, 30, 30);
}
//
//function used to call features
void showCheat(int xres, int yres) {
    showFeature(xres, yres);
    drawCheatCode(xres, yres);
    greenCircle(xres, yres);
}

