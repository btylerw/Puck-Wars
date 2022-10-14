//Aldair Martinez
//new source file
//
#include <iostream>
#include <GL/glx.h>
#include "amartinez2.h"

using namespace std;
/*
typedef struct t_button {
	Rect r;
} Button;
*/
void show_name() {
	printf("aldair\n");
	//return 0;
}
//global
//create unsigned var
//put var in case statements credits
//gl.credits =^ 1			(replace with manage state)
//gl.credits = manage_state(gl.credits)
//void function show credits
//show credits function is called in render
//	if gl.credits 
//		show_credits()
//create header file
//		amartinez2.h
//int main.cpp file, add: 
//		#include "amartinez2.h"
//	add amartinez2.h in Make file

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
