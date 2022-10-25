//Tyler Brown, Andres Botello, Aldair Martinez, Anh Vu, Abisai Diaz Perez
//
//3350 Group 7 main source code file
//Modified version of Waterfall framework
//for an air hockey like game
//
//author: Gordon Griesel
//date: Spring 2022
//purpose: get openGL working on your personal computer
//
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "log.h"
#include "tbrown.h"
#include "abotello.h"
#include "avu2.h"
#include "amartinez2.h"
//some structures

// Define some global variables
class Global {
    public:
	int xres, yres;
	int mouse_x, mouse_y;
	int pressed;
	int intro_screen;
	unsigned int credit;
	unsigned int feature;
	unsigned int pause;
	int help_screen;
	Global()
	{
	    xres = 600;
	    yres = 1200;
	    pressed = 0;
	    mouse_x = 0;
	    mouse_y = 0;
	    pause = 0;
	    intro_screen = 1;
	    credit = 0;
	    feature = 0;
	    help_screen = 0;
	}
} gl;

// Get image data
class Image {
    public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
	    if (fname[0] == '\0')
		return;
	    int ppmFlag = 0;
	    char name[40];
	    strcpy(name, fname);
	    int slen = strlen(name);
	    char ppmname[80];
	    if (strncmp(name+(slen-4), ".ppm", 4) == 0)
		ppmFlag = 1;
	    if (ppmFlag) {
		strcpy(ppmname, name);
	    } else {
		name[slen-4] = '\0';
		sprintf(ppmname, "%s.ppm", name);
		char ts[100];
		sprintf(ts, "convert %s %s", fname, ppmname);
		system(ts);
	    }
	    FILE *fpi = fopen(ppmname, "r");
	    if (fpi) {
		char line[200];
		fgets(line, 200, fpi);
		fgets(line, 200, fpi);
		while (line[0] == '#' || strlen(line) < 2)
		    fgets(line, 200, fpi);
		sscanf(line, "%i %i", &width, &height);
		fgets(line, 200, fpi);
		int n = width * height * 3;
		data = new unsigned char[n];
		for (int i = 0; i < n; i++)
		    data[i] = fgetc(fpi);
		fclose(fpi);
	    } else {
		printf("ERROR opening image: %s\n", ppmname);
		exit(0);
	    }
	    if (!ppmFlag)
		unlink(ppmname);
	}
};

Image img[5] = {"./intro.png", "./background.png", 
    "./credit.png", "./help_screen.png", 
    "pause.png" };

GLuint introTexture;
GLuint backgroundTexture;
GLuint creditTexture;
GLuint helpTexture;
GLuint pauseTexture;
Image *introImage = NULL;
Image *backgroundImage = NULL;
Image *creditImage = NULL;
Image *helpImage = NULL;
Image *pauseImage = NULL;


class Box {
    public:
	float vel[2];
	float w;
	float h;
	float x;
	float y;
	float dir;
	float pos[2];
	Box() {
	    h = 12.0f;
	    w = 30.0f;
	    pos[0] = x = gl.xres / 2.0;
	    pos[1] = y = gl.yres / 4;
	    vel[0] = vel[1] = 0.0;
	}
	Box(double width, double p0, double p1, double v1) {
	    w = width;
	    pos[0] = p0;
	    pos[1] = p1;
	    vel[0] = vel[1] = v1;
	}
};

Box paddle;
Box puck(10, gl.xres / 2, gl.yres / 1.2, 0.0);
int n = 0;

class X11_wrapper {
    private:
	Display *dpy;
	Window win;
	GLXContext glc;
    public:
	~X11_wrapper();
	X11_wrapper();
	void set_title();
	bool getXPending();
	XEvent getXNextEvent();
	void swapBuffers();
	void reshape_window(int width, int height);
	void check_resize(XEvent *e);
	void check_mouse(XEvent *e);
	int check_keys(XEvent *e);
} x11;

//Function prototypes
void init_opengl(void);
void physics(void);
void render(void);


//=====================================
// MAIN FUNCTION IS HERE
//=====================================
int main()
{
    init_opengl();
    //Main loop
    int done = 0;
    while (!done) {
	//Process external events.
	while (x11.getXPending()) {
	    XEvent e = x11.getXNextEvent();
	    x11.check_resize(&e);
	    x11.check_mouse(&e);
	    done = x11.check_keys(&e);
	}
	physics();
	render();
	x11.swapBuffers();
	usleep(200);
    }
    cleanup_fonts();
    return 0;
}


X11_wrapper::~X11_wrapper()
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

X11_wrapper::X11_wrapper()
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    int w = gl.xres, h = gl.yres;
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
	cout << "\n\tcannot connect to X server\n" << endl;
	exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
	cout << "\n\tno appropriate visual found\n" << endl;
	exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask =
	ExposureMask | KeyPressMask | KeyReleaseMask |
	ButtonPress | ButtonReleaseMask |
	PointerMotionMask |
	StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
	    InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
}

void X11_wrapper::set_title()
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Puck Wars");
}

bool X11_wrapper::getXPending()
{
    //See if there are pending events.
    return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
    //Get a pending event.
    XEvent e;
    XNextEvent(dpy, &e);
    return e;
}

void X11_wrapper::swapBuffers()
{
    glXSwapBuffers(dpy, win);
}

void X11_wrapper::reshape_window(int width, int height)
{
    //window has been resized.
    gl.xres = width;
    gl.yres = height;
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
}

void X11_wrapper::check_resize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
	return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != gl.xres || xce.height != gl.yres) {
	//Window size did change.
	reshape_window(xce.width, xce.height);
    }
}
//-----------------------------------------------------------------------------


void X11_wrapper::check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;

    //Weed out non-mouse events
    if (e->type != ButtonRelease &&
	    e->type != ButtonPress &&
	    e->type != MotionNotify) {
	//This is not a mouse event that we care about.
	return;
    }
    //
    if (e->type == ButtonRelease) {
	return;
    }
    if (e->type == ButtonPress) {
	while (e->xbutton.button==1) {
	    //Left button was pressed.
	    return;
	}
	if (e->xbutton.button==3) {
	    //Right button was pressed.
	    return;
	}
    }
    if (e->type == MotionNotify) {
	//The mouse moved!
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
	    savex = e->xbutton.x;
	    savey = e->xbutton.y;
	    // Saving mouse coordinates
	    gl.mouse_x = savex;
	    gl.mouse_y = gl.yres - savey;
	    //Code placed here will execute whenever the mouse moves.
	}
    }
}

int X11_wrapper::check_keys(XEvent *e)
{
    if (e->type != KeyPress && e->type != KeyRelease)
	return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
	switch (key) {
	    case XK_space:
		gl.pressed = 1;
		break;
	    case XK_Escape:
		//Escape key was pressed
		return 1;
	    case XK_Return:
		gl.intro_screen = 0;
		break;
	    case XK_p:
		gl.pause = manage_pstate(gl.pause);
		break;
	    case XK_F1:
		if (gl.help_screen)
		    gl.help_screen = 0;
		else
		    gl.help_screen = 1;
		break;
	    case XK_c:
		gl.credit += 1;
		if (gl.credit > 1) {
		    gl.credit = 0;
		}
		break;
	    case XK_f:
		gl.feature += 1;
		if (gl.feature > 1) {
		    gl.feature = 0;
		}
		break;
	    case XK_i:
		gl.intro_screen = !gl.intro_screen;
		break;
	}
    }
    return 0;
}

// Resets everything to starting positions
void reset()
{
    puck.pos[0] 	= gl.xres / 2;
    puck.pos[1] 	= gl.yres / 1.2;
    puck.vel[0] 	= puck.vel[1] = 0;
    paddle.pos[0] 	= gl.xres /2;
}

void init_opengl(void)
{
    int h, w; 
    //OpenGL initialization
    glViewport(0, 0, gl.xres, gl.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    //Needed for text to be added
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

    // Load images
    introImage 		= &img[0];
    backgroundImage = &img[1];
    creditImage 	= &img[2];
    helpImage 		= &img[3];
    pauseImage      = &img[4];
    //
    // Allocate opengl texture identifiers
    glGenTextures(1, &introTexture);
    glGenTextures(1, &backgroundTexture);
    glGenTextures(1, &creditTexture);
    glGenTextures(1, &helpTexture);
    glGenTextures(1, &pauseTexture);
    //
    // Load textures into memory	
    // Intro Image ============================================================
    w = introImage->width;
    h = introImage->height;
    glBindTexture(GL_TEXTURE_2D, introTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, introImage->data);
    // ========================================================================
    // Background Image =======================================================
    w = backgroundImage->width;
    h = backgroundImage->height;
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, backgroundImage->data);
    glBindTexture(GL_TEXTURE_2D, 0);
    // ========================================================================
    // Credit Image =======================================================
    w = creditImage->width;
    h = creditImage->height;
    glBindTexture(GL_TEXTURE_2D, creditTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, creditImage->data);
    glBindTexture(GL_TEXTURE_2D, 0);
    // ========================================================================
    // Help Screen Image =======================================================
    w = helpImage->width;
    h = helpImage->height;
    glBindTexture(GL_TEXTURE_2D, helpTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, helpImage->data);
    glBindTexture(GL_TEXTURE_2D, 0);
    // ========================================================================
    // Pause Image =======================================================
    w = pauseImage->width;
    h = pauseImage->height;
    glBindTexture(GL_TEXTURE_2D, pauseTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, pauseImage->data);
    glBindTexture(GL_TEXTURE_2D, 0);
    // ========================================================================
}

void physics()
{
    // pauses game
    if (!gl.pause){
	// Saving previous paddle pos for velocity calculation
	int old_pos  = paddle.pos[1];

	// Updates paddle position with new mouse coordinates
	paddle.pos[0] = gl.mouse_x;
	paddle.pos[1] = gl.mouse_y;
	if (paddle.pos[1] > 300)
	    paddle.pos[1] = 300;
	paddle.vel[1] = (paddle.pos[1] - old_pos) / 1.5;
	// Puck begins to move
	if (gl.pressed) {
	    puck.pos[0] += puck.vel[0];
	    puck.pos[1] += puck.vel[1];
	    puck.vel[1] -= 0.1;
	}
	// Checking for wall collision
	if (puck.pos[0] - puck.w < 0)
	    puck.vel[0] = -puck.vel[0];

	else if (puck.pos[0] + puck.w > gl.xres)
	    puck.vel[0] = -puck.vel[0];

	if (puck.pos[1] + puck.w > gl.yres)
	    puck.vel[1] = -puck.vel[1];

	// If puck falls below screen, reset game
	if (puck.pos[1] - puck.w < 0) {
	    gl.pressed = 0;
	    reset();
	}

	// Check for puck/paddle collision
	// Adds paddle velocity to puck velocity
	if ((puck.pos[1] - puck.w) < (paddle.pos[1] + paddle.h) &&
		puck.pos[1] > (paddle.pos[1] - paddle.h) &&
		puck.pos[0] > (paddle.pos[0] - paddle.w) &&
		puck.pos[0] < (paddle.pos[0] + paddle.w)) {
	    if (puck.pos[0] > paddle.pos[0]) {
		puck.vel[1] = 0;
		puck.pos[1] = paddle.pos[1] + paddle.h;
		puck.vel[1] += paddle.vel[1];
		puck.vel[0] = 2;
		if (paddle.vel[1] <= 0)
		    puck.vel[1] = 0;
	    }

	    else if (puck.pos[0] < paddle.pos[0]) {
		puck.vel[1] = 0;
		puck.pos[1] = paddle.pos[1] + paddle.h;
		puck.vel[1] += paddle.vel[1];
		puck.vel[0] = -2;
		if (paddle.vel[1] <= 0)
		    puck.vel[1] = 0;
	    }

	    else {
		puck.vel[1] = 0;
		puck.pos[1] = paddle.pos[1] + paddle.h;
		puck.vel[1] += paddle.vel[1];
		if (paddle.vel[1] <= 0) {
		    float new_vel = -puck.vel[1] / 3;
		    puck.vel[1] = 0;
		    puck.vel[1] = new_vel;
		}
	    }
	}
    }
}


void render()
{
    Rect r;
    glViewport(0, 0, gl.xres, gl.yres);
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    glColor3f(1.0f, 1.0f, 1.0f);
    // Render background
    show_background(backgroundTexture, gl.xres, gl.yres);
    // Define positions for instruction text
    r.bot = gl.yres/1.5;
    r.left = gl.xres/2;
    r.center = -5;
    if (!gl.pressed) {
	ggprint8b(&r, 16, 0x00ff0000, "PRESS SPACE TO START");
    }
    // Draw paddle
    glPushMatrix();
    glColor3ub(100, 200, 100);
    glTranslatef(paddle.pos[0], paddle.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-paddle.w, -paddle.h);
    glVertex2f(-paddle.w,  paddle.h);
    glVertex2f(paddle.w, paddle.h);
    glVertex2f(paddle.w, -paddle.h);
    glEnd();
    glPopMatrix();
    // Draw puck
    glPushMatrix();
    glColor3ub(150, 160, 220);
    glTranslatef(puck.pos[0], puck.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-puck.w, -puck.w);
    glVertex2f(-puck.w, puck.w);
    glVertex2f(puck.w, puck.w);
    glVertex2f(puck.w, -puck.w);
    glEnd();
    glPopMatrix();
    if (gl.intro_screen) {
	show_intro_screen(introTexture, gl.xres, gl.yres);
    }

    if (gl.pause) {
	pause_screen(pauseTexture, gl.xres, gl.yres);
    }

    if (gl.help_screen) {
	help_screen(helpTexture, gl.xres, gl.yres);
    }

    if (gl.feature != 0) {
	showFeature(gl.xres, gl.yres);
    }

    if (gl.credit != 0) {
	showCredit(creditTexture, gl.xres, gl.yres);
	r.bot = gl.yres-50;
	ggprint16(&r, 0, 0x0088aaff, "Credit Screen");
	r.bot = gl.yres-100;
	ggprint16(&r, 0, 0x0088aaff, "Tyler, Andres, Aldair, Anh, Abisai");
    }
}
