/* Phillip Lane
 * CS 545 01
 * 9/2/2020
 * Dr. Hauenstein
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

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

unsigned int samples = 5;
int offset = 0;
float *lines;
float y[] = {
	0.0F, 50.0F, 0.0F, 100.0F, 50.0F, 0.0F
};
bool isSine = true;

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, y);

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < 2; i++)
		glArrayElement(i);
	glEnd();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, lines);

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < samples; i++)
		glArrayElement(i);
	glEnd();
	
	glFlush();
}

void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	
	if (lines != NULL)
		free(lines);
	
	lines = (float*)malloc(sizeof(float) * 3 * samples + 3);

	int j = 0;
	for (float i = 0; i <= 2 * M_PI + 0.001; i += 2 * (float)M_PI / (float)(samples - 1))
	{
		lines[3 * j + 0] = 50 * i / (float)M_PI;
		if (isSine)
			lines[3 * j + 1] = 50 * (float)sin((double)i + M_PI / 8 * offset) + 50;
		else
			lines[3 * j + 1] = 50 * (float)cos((double)i + M_PI / 8 * offset) + 50;
		printf("%f\n", lines[3 * j + 1]);
		lines[3 * j + 2] = 0;
		j++;
	}
}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
		samples++;
		setup();
		glutPostRedisplay();
		break;
	case '-':
		samples = samples > 3 ? samples - 1 : samples;
		setup();
		glutPostRedisplay();
		break;
	case ' ':
		isSine = !isSine;
		setup();
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
	default: break;
	}
}

void specialKeyInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		offset--;
		setup();
		glutPostRedisplay();
		break;
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
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sine and Cosine Waves");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	lines = (float*)NULL;
	setup();

	glutMainLoop();
}