/* Phillip Lane
 * CS 545 01
 * 9/2/2020
 * Dr. Hauenstein
 *
 * Software used:
 * Windows 10 Pro
 * Visual Studio 2017 (v141)
 * Windows SDK Toolkit 10.0.18362.0
 * 
 * Hardware used:
 * 2x Opteron 6380
 * 64GB DDR3
 * Nvidia Quadro K4200
 *
 * This program renders sine and cosine waves with variable sampled points on each wave, with a default of 5 samples. The samples are equidistant on the
 * waves. The amplitute of the wave reaches exactly the top and bottom of the viewing window, and the period matches exactly the width of the viewing window.
 *
 * Implemented with GL_LINE_STRIP and vertex arrays, hence calls to glVertexPointer and glArrayElement instead of glVertex3f.
 *
 * Controls:
 *   +   : Increase samples on the wave by 1 (no maximum)
 *   -   : Decrease samples on the wave by 1 (minimum of 3)
 *   <-  : Shift wave left by pi/8
 *   ->  : Shift wave right by pi/8
 * space : Toggle between sine and cosine
 *  esc  : Quit
 */

//Just some libraries we're gonna be using
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

//How many samples we're taking
unsigned int samples = 5;
//Offset of the wave
int offset = 0;
//Pointer to a block of contiguous memory (allocated later) that contains the vertices for the GL_LINE_STRIP
float *lines;
//Vertices for the y axis (of which there are 2 :p)
float y[] = {
	0.0F, 50.0F, 0.0F, 100.0F, 50.0F, 0.0F
};
//Boolean determining whether we draw sine or cosine
bool isSine = true;

//Handler for drawing the scene
void drawScene(void)
{
	//Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Bind to the vertex array the vertices of the y axis
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, y);

	//Set the color to black
	glColor3f(0.0, 0.0, 0.0);
	//Draw the line strip
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < 2; i++)
		glArrayElement(i);
	glEnd();

	//Bind the vertex array to the vertices of the wave
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, lines);

	//Set the color to a nice pretty purple :)
	glColor3f(0.7, 0.0, 1.0);
	//Draw the wave as a line strip
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < samples; i++)
		glArrayElement(i);
	glEnd();

	//Flush everything to force it to get drawn
	glFlush();
}

//Resizing handler
void resize(int width, int height)
{
	//Set the viewing box
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Set up the vertex array for drawing
void setup(void)
{
	//Clear the color to white (I think this does a white background)
	glClearColor(1.0, 1.0, 1.0, 0.0);

	//If we have not allocated an array for our line vertices, don't free it (I kinda wish I had a Ruby "unless" clause here)
	//like: free(lines) unless (lines == NULL);
	if (lines != NULL)
		free(lines);

	//Allocate as many vertices as samples, with an extra vertex at the end in case the floating point math goes wonky (which could totally happen) to prevent a random segfault
	lines = (float*)malloc(sizeof(float) * 3 * samples + 3);

	//Our little buddy here is gonna help us determine where in the lines array to store our floats
	//Good luck on your journey ^^
	int j = 0;
	//Iterate through all the sample points
	for (float i = 0; i <= 2 * M_PI + 0.001; i += 2 * (float)M_PI / (float)(samples - 1))
	{
		//Set the x axis, scaled to fit inside the viewing box
		lines[3 * j + 0] = 50 * i / (float)M_PI;
		//Determine if we're drawing a sine or a cosine wave
		if (isSine)
			//Set the sine y axis, scaled to fit inside the viewing box, and offset by however much we need
			lines[3 * j + 1] = 50 * (float)sin((double)i + M_PI / 8 * offset) + 50;
		else
			//Same for the cosine
			lines[3 * j + 1] = 50 * (float)cos((double)i + M_PI / 8 * offset) + 50;
		//Set the z axis, which is always zero
		lines[3 * j + 2] = 0;
		//Increment j
		j++;
	}
}

//Handler for ASCII input
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		//If +, increase the samples, recreate the line, and redraw
	case '+':
		samples++;
		setup();
		glutPostRedisplay();
		break;
		//If -, decrease the samples (checking to make sure we don't go under 3), recreate the line, and redraw
	case '-':
		samples = samples > 3 ? samples - 1 : samples;
		setup();
		glutPostRedisplay();
		break;
		//If space, toggle the isSine boolean, recreate the line, and redraw
	case ' ':
		isSine = !isSine;
		setup();
		glutPostRedisplay();
		break;
		//If escape, quit
	case 27:
		exit(0);
	default: break;
	}
}

//Handler for non-ASCII input
void specialKeyInput(int key, int x, int y)
{
	switch (key)
	{
		//If left arrow, decrease the offset, recreate the line, and redraw
	case GLUT_KEY_LEFT:
		offset--;
		setup();
		glutPostRedisplay();
		break;
		//If right arrow, increase the offset, recreate the line, and redraw
	case GLUT_KEY_RIGHT:
		offset++;
		setup();
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int main(int argc, char **argv)
{
	//Initialize the command line arguments
	glutInit(&argc, argv);

	//Set context version to 4.0 so that it'll play nicely with Dr. Hauenstein's PC (he can't run OpenGL 4.3)
	glutInitContextVersion(4, 0);
	//Compatibility profile, so that we can do glBegin and glEnd, among other things
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	//Set the window options, like display mode, size, position, and text
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sine and Cosine Waves");

	//Attach our handler functions
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	//Set this because glew doesn't like it when you don't, and initialize glew
	glewExperimental = GL_TRUE;
	glewInit();

	//Set lines to NULL and go to the setup function
	lines = (float*)NULL;
	setup();

	//Start the main glut loop! We're off to the races!!
	glutMainLoop();
}