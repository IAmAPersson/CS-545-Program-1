#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define SINE 0
#define COSINE 1
#define Y 2

#define MAXSAMPLES 250

unsigned int vao[3];
unsigned int buffer;
float sine[3 * MAXSAMPLES];
float cosine[3 * MAXSAMPLES];
float y[] = {
	50.0, 0.0, 0.0, 50.0, 100.0, 0.0
};

void drawScene()
{

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

void setup()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glGenVertexArrays(3, vao);

	glBindVertexArray(vao[SINE]);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, &buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sine), sine, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindVertexArray(vao[COSINE]);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, &buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cosine), cosine, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindVertexArray(vao[Y]);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, &buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(y), y, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboarFunc(keyInput);
}