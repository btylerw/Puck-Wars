//Abisai Diaz Perez
#include <GL/glx.h>
#include <cstdlib>
#include <iostream>
#include <time.h>

using namespace std;

GLfloat r = 0.0f;
GLfloat g = 1.0f;
GLfloat b = 1.0f;

GLfloat colorcheck(GLfloat a);

//create trail
void trail(int originx, int originy, float *velocity, bool choice)
{
    int mod, mod2;
    mod = mod2 = 1;
    if(velocity[0] >= 0)
        mod = -1;
    if(velocity[1] >= 0)
        mod2 = -1;
    float x = originx + (mod * velocity[0] * 3);
    float y = originy + (mod2 * velocity[1] * 3);
    //y velocity is negative, trail points up    
    if(mod2 < 0) {
            glColor3f(r, g, b);
            glColor3f(r, g, b);
            if(choice == 0) {
                r += 0.1;
                g += 0.1;
                b += 0.1;
            } else {
                if(abs(velocity[1]) < 4) {
                r = 0.0;
                g = 0.0;
                b = 1.0;
            } else if(4 <= abs(velocity[1]) && abs(velocity[1]) <= 13) {
                r = 0.0;
                g = 1.0;
                b = 0.0;
            } else if(abs(velocity[1]) > 13) {
                r = 1.0;
                g = 0.0;
                b = 0.0;
            }
            }
            r = colorcheck(r);
            g = colorcheck(g);
            b = colorcheck(b);
            for(int i=0; i < 30; i++)
            {
                //creat triangle origin points relative to the puck, then
                //create triangle vertices
                glBegin(GL_TRIANGLES);
                int temp = originx + (i * mod) + ((rand() % 4) * mod);
                int temp2 = originy + (i * mod2) + ((rand() % 4) * mod2);
                glVertex2f(temp - 2, temp2 - 2);
                glVertex2f(temp, temp2-3);
                glVertex2f(temp-2, temp2-3);
                glEnd();
            }
    } else {
        //y velocity is positive, trail points down
        glColor3f(r,g,b);
        if(choice = 0) {
            r += 0.1;
            g += 0.1;
            b += 0.1;
        } else {
            if(abs(velocity[1]) < 4) {
                r = 0.0;
                g = 0.0;
                b = 1.0;
            } else if(4 <= abs(velocity[1]) && abs(velocity[1]) <= 13) {
                r = 0.0;
                g = 1.0;
                b = 0.0;
            } else if(abs(velocity[1]) > 13) {
                r = 1.0;
                g = 0.0;
                b = 0.0;
            }
            
        }
        r = colorcheck(r);
        g = colorcheck(g);
        b = colorcheck(b);
        for(int i = 30; i > 0; i--)
        {
            //same as before, but down
            glBegin(GL_TRIANGLES);
            int temp = originx + (i * mod) + ((rand() % 4) * mod);
            int temp2 = originy + i + ((rand() % 4) * mod2) + 4;
            glVertex2f(temp - 2, temp2 - 2);
            glVertex2f(temp, temp2-3);
            glVertex2f(temp-2, temp2-3);
            glEnd();
        } 
    }
}

GLfloat colorcheck(GLfloat a)
{
    if(a > 1.0)
        a = 0.0;
    return a;
}
