#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define SINE 0
#define COSINE 1
#define Y 2

#define MAXSAMPLES 250

unsigned int vao[3];
unsigned int buffer;
unsigned int samples = 5;
float sine[3 * MAXSAMPLES + 3];
float cosine[3 * MAXSAMPLES + 3];
float y[] = {
	0.0, 50.0, 0.0, 100.0, 50.0, 0.0
};

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vao[Y]);
	glColor3f(0.0, 0.0, 0.0);
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	glBindVertexArray(vao[SINE]);
	glColor3f(1.0, 0.0, 0.0);
	glDrawArrays(GL_LINE_STRIP, 0, 251);

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
	int j = 0;
	for (float i = 0; i < 2 * M_PI; i += 2 * (float)M_PI / MAXSAMPLES)
	{
		sine[3 * j + 0] = 50 * i / M_PI;
		sine[3 * j + 1] = 50 * (float)sin(i) + 50;
		printf("%f, %f\n", i, sine[3 * j + 1]);
		sine[3 * j + 2] = 0;
		j++;
	}

	j = 0;
	for (float i = 0; i < 2 * M_PI; i += 2 * (float)M_PI / MAXSAMPLES)
	{
		cosine[3 * j + 0] = 50 * i / M_PI;
		cosine[3 * j + 1] = 100 * (float)cos(i) + 50;
		cosine[3 * j + 2] = 0;
		j++;
	}

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glGenVertexArrays(3, vao);

	glBindVertexArray(vao[SINE]);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sine), sine, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindVertexArray(vao[COSINE]);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cosine), cosine, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindVertexArray(vao[Y]);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(y), y, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);
}

void keyInput(unsigned char key, int x, int y)
{

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sine and Cosine Waves");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}