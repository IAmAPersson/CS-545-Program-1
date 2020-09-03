#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define SINE 0
#define COSINE 1
#define Y 2

#define MAXSAMPLES 250

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
	
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.0, 0.0);
	for (unsigned int i = 0; i < 2; i++)
		glArrayElement(i);
	glEnd();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, lines);

	glBegin(GL_LINE_STRIP);
	glColor3f(0.1, 0.0, 0.0);
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