// Quiz5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <ImathVec.h>
#include<iostream>
#include<fstream>
#include<string>
#include <direct.h>  
#include <GL/glut.h>

#define PI 3.1415926
using namespace std;
using namespace Imath;

// mesh
typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;
struct Mesh {
	// vertex
	vector<Vec3f> dot_vertex;
	vector<Vec3f> dot_normal;
	vector<Vec2f> dot_texture;
	// faces
	vector<int> face_index_vertex;
	vector<int> face_index_normal;
	vector<int> face_index_texture;
};

// global
int width = 1200;
int height = 600;
float rratio = 1.0;
GLuint displayList;
int N = 20;

// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 1.0;
float x_angle = 0.0;
float y_angle = 0.0;
// global moving
float angle = 0;
Vec3f moving_position = Vec3f(0, 0, 0);
Vec3f center_position = Vec3f(0, 0, 0);

float Points[4][3] = {
	{10, 0, 0},
	{0, 0, 0},
	{0, 10, 0},
	{5, 10, 0}
};

/**
* init
*/
void init() {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	// <JGS> step 3 create more Mesh variables <JGS>
	// <JGS> another_displayList = meshToDisplayList(another_mesh,2); </JGS>
	rratio = (double)width / (double)height;
}

/**
* reshape
*/
void reshape(int w, int h) {
	width = w;
	height = h;
	if (h == 0) h = 1;
	rratio = 1.0f * w / h;
}

/**
* move
*/
void move() {
	// rotate diamond
	angle = angle + 0.1;
	if (angle > 360) angle -= 360;
	moving_position.x = center_position.x + 8.0f * sin(angle * PI / 180);
	moving_position.z = center_position.z + 8.0f * cos(angle * PI / 180);
	glutPostRedisplay();
}

/**
* renderBitmapString
*/
void renderBitmapString(float x, float y, float z, const char *string) {
	const char *c;
	glRasterPos3f(x, y, z); // fonts position
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

/**
* display
*/
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45, rratio, 1, 1000);
	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// lookAt fijo
	gluLookAt(0.0f, 0.0f, 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f);
	// camera
	glScalef(scale, scale, scale);
	glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);

	glColor3f(1, 0, 0);
	glPointSize(3);
	glBegin(GL_POINTS);
	for (int i = 0; i != 4; ++i) {
		glVertex3fv(Points[i]);
	}
	glEnd();
	glColor3f(0, 0, 1);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i != 4; ++i) {
		glVertex3fv(Points[i]);
	}
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 0);
	//N = 20
	for (int i = 0; i != N; ++i) {
		float t = (float)i / (N - 1);
		float it = 1.0f - t;
		//blending
		float b0 = t*t*t;
		float b1 = 3 * t * t * it;
		float b2 = 3 * t * it * it;
		float b3 = it * it * it;
		//P(t)
		float x = b3 * Points[0][0] + b2 * Points[1][0] + b1 * Points[2][0] + b0 * Points[3][0];
		float y = b3 * Points[0][1] + b2 * Points[1][1] + b1 * Points[2][1] + b0 * Points[3][1];
		float z = b3 * Points[0][2] + b2 * Points[1][2] + b1 * Points[2][2] + b0 * Points[3][2];
		//specify the point
		glVertex3f(x, y, z);
	}
	glEnd();


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glutSwapBuffers();
	move();
}

/**
* main
*/
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Quiz 5");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}