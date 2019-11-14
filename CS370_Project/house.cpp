// CS370 - Fall 2019
// Final Project
//Daniel Palmieri T/TR 12:30-1:45
//Room currently contains basic geometry for a fireplace, fan, table with chairs, door, mirror, christmas tree, and window.
#ifdef OSX
	#include <GLUT/glut.h>
#else
	#include <GL/glew.h>
	#include <GL/glut.h>
#endif
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lighting.h"
#include "materials.h"
#include "globals.h"

// Projection states
#define ORTHOGRAPHIC 0
#define FIRSTPERSON 1
int projectionType = ORTHOGRAPHIC;

// Shader file utility functions
#include "shaderutils.h"
#define X 0
#define Y 1
#define Z 2
#define RAD2DEG (180.0f/3.14159f)
#define DEG2RAD (3.14159f/180.0f)

// lists
#define CUBE 1
#define WALL 2
#define ROOM 3
#define TABLE_CHAIRS 4
#define MIRROR 5
#define FAN 6

// Color identifiers
#define RED 0
#define GREEN 1
#define BLUE 2
#define GRAY 3
#define YELLOW 4
#define TIE_COLOR 5
#define PINK 6
#define CYAN 7
#define VIOLET 8
#define BLACK 9
#define BROWN 10
// Vertex colors
GLfloat current_color[][3] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f, 0.5f}, { 1.0f, 1.0f, 0.0f } ,{0.69f, 0.13f, 0.13f}, {1.0f,0.35f,0.39f},{0.0f, 1.0f, 1.0f},
{0.93f,0.51f , 0.93f}, {0.0f, 0.0f, 0.0f}, {0.82f, 0.41f, 0.11f} };
GLint color = RED;

// Global camera vectors
GLfloat eye[3] = { 1.0f,1.0f,1.0f };
GLfloat at[3] = { 0.0f,0.0f,0.0f };
GLfloat up[3] = { 0.0f,1.0f,0.0f };

GLfloat eye_p[3] = { 0, 5, 0 };
GLfloat at_p[3] = { 0, 0, 1 };
GLfloat up_p[3] = { 0, 1, 0 };
GLfloat eye_p_new[3] = {0, 0, 0};

// Global screen dimensions
GLint ww, hh;


// Shader files
GLchar* defaultVertexFile = "defaultvert.vs";
GLchar* defaultFragmentFile = "defaultfrag.fs";
GLchar* lightVertexFile = "lightvert.vs";
GLchar* lightFragmentFile = "lightfrag.fs";
GLchar* texVertexFile = "texturevert.vs";
GLchar* texFragmentFile = "texturefrag.fs";

// Shader objects
GLuint defaultShaderProg;
GLuint lightShaderProg;
GLuint textureShaderProg;
GLuint numLights_param;
GLint numLights = 1;
GLint texSampler;

void display();
void render_Scene();
void keyfunc(unsigned char key, int x, int y);
void idlefunc();
void reshape(int w, int h);
void create_lists();
void setColor(GLint colorID);

int main(int argc, char *argv[])
{
	// Initialize GLUT
	glutInit(&argc,argv);

	// Initialize the window with double buffering and RGB colors
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Set the window size to image size
	glutInitWindowSize(512,512);

	// Create window
	glutCreateWindow("House");

#ifndef OSX
	// Initialize GLEW - MUST BE DONE AFTER CREATING GLUT WINDOW
	glewInit();
#endif

	// Define callbacks
	glutDisplayFunc(display);
	glutKeyboardFunc(keyfunc);
	glutIdleFunc(idlefunc);
	glutReshapeFunc(reshape);

	
	// Set background color to white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Load shader programs
	defaultShaderProg = load_shaders(defaultVertexFile,defaultFragmentFile);
	lightShaderProg = load_shaders(lightVertexFile, lightFragmentFile);
	textureShaderProg = load_shaders(texVertexFile, texFragmentFile);
	numLights_param = glGetUniformLocation(lightShaderProg, "numLights");
	texSampler = glGetUniformLocation(textureShaderProg, "texMap");
	glUseProgram(defaultShaderProg);

	//create lists
	create_lists();

	// Begin event loop
	glutMainLoop();
	return 0;
}

// Display callback
void display()
{
	// Reset background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Adjust viewing volume (orthographic)
	GLfloat xratio = 1.0f;
	GLfloat yratio = 1.0f;
	// If taller than wide adjust y
	if (ww <= hh)
	{
		yratio = (GLfloat)hh / (GLfloat)ww;
	}
	// If wider than tall adjust x
	else if (hh <= ww)
	{
		xratio = (GLfloat)ww / (GLfloat)hh;
	}
	glOrtho(-25.0f*xratio, 25.0f*xratio, -25.0f*yratio, 25.0f*yratio, -25.0f, 25.0f);


	if (projectionType == ORTHOGRAPHIC) {
		// Set modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye[X], eye[Y], eye[Z], at[X], at[Y], at[Z], up[X], up[Y], up[Z]);
	}
	else if (projectionType == FIRSTPERSON) {
		// Set modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//gluLookAt(new coordinates)
	}


	// Render scene
	render_Scene();

	// Flush buffer
	glFlush();

	// Swap buffers
	glutSwapBuffers();
}

// Scene render function
void render_Scene()
{
	// draw room
	glPushMatrix();
	glCallList(ROOM);
	glPopMatrix();
	
	// table and chairs
	setColor(RED);
	glPushMatrix();
	glTranslatef(-11.0f, -1, 0);
	glCallList(TABLE_CHAIRS);
	glPopMatrix();

	// mirror
	glPushMatrix();
	glCallList(MIRROR);
	glPopMatrix();

	// Christmas tree
	glPushMatrix();
	setColor(GREEN);
	glTranslatef(tree_offset, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.75, 0.75, 0.75);
	glutSolidCone(tree_base, tree_height, tree_slices, tree_stacks);
	glPopMatrix();

	glPushMatrix();
	setColor(BROWN);
	GLUquadricObj* tree_stump;
	tree_stump = gluNewQuadric();
	gluQuadricDrawStyle(tree_stump, GLU_LINE);
	glTranslatef(tree_offset, -3, 0);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(tree_stump, stump_radius,stump_radius, stump_height, stump_slices, stump_stacks);
	glPopMatrix();

	// door
	glPushMatrix();
	glTranslatef(0, -0.5, (-wall_length/2));
	glScalef(1, 1, 0.1);
	glutSolidCube(5.0);
	glPopMatrix();

	// fan
	glPushMatrix();
	glCallList(FAN);
	glPopMatrix();
	
	// fireplace
	glPushMatrix();
	setColor(BROWN);
	glTranslatef(-wall_length+1.05, -1.0, 0);
	glScalef(0.4, 0.7, 1);
	glutSolidCube(5.0);
	glPopMatrix();
	

	glPushMatrix();
	setColor(RED);
	glTranslatef(-wall_length + 1.0 ,-0.5, 0);
	glScalef(0.4, 1, 1.5);
	glutSolidCube(5.0);
	glPopMatrix();

}

// Keyboard callback
void keyfunc(unsigned char key, int x, int y)
{
	if (projectionType == ORTHOGRAPHIC) {
		if (key == 'a')
		{
			azimuth += daz;
			if (azimuth > 360.0)
			{
				azimuth -= 360.0;
			}
		}
		if (key == 'd')
		{
			azimuth -= daz;
			if (azimuth < 0.0)
			{
				azimuth += 360.0;
			}
		}

		// TODO: Adjust elevation angle
		if (key == 'w')
		{
			elevation += del;
			if (elevation > 180.0)
			{
				elevation = 180.0;
			}
		}
		if (key == 's')
		{
			elevation -= del;
			if (elevation < 0.0)
			{
				elevation = 0.0;
			}
		}
		

		// TODO: Compute cartesian camera position
		eye[X] = (GLfloat)(radius * sin(azimuth * DEG2RAD) * sin(elevation * DEG2RAD));
		eye[Y] = (GLfloat)(radius * cos(elevation * DEG2RAD));
		eye[Z] = (GLfloat)(radius * cos(azimuth * DEG2RAD) * sin(elevation * DEG2RAD));
	}
	else if (projectionType == FIRSTPERSON) {
		// w to move forwards
		// s to move backwards
		// a to pivot left
		// d to pivot right
	}

	//eyePerspective[X]
	//eyePerspective[Y]
	//eyePerspective[Z]
	// <esc> quits
	if (key == 27)
	{
		exit(0);
	}

	if (key == 'p' || key == 'P') {
		projectionType = !projectionType;
	}

}

// Idle callback
void idlefunc()
{
	glutPostRedisplay();
}

// Reshape callback
void reshape(int w, int h)
{
	// Set new screen extents
	glViewport(0, 0, w, h);

	// Store new extents
	ww = w;
	hh = h;
}


void create_lists() 
{
	// cube list
	glNewList(CUBE, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glutSolidCube(1.0);
	glPopAttrib();
	glEndList();

	// wall list
	glNewList(WALL, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glScalef(1, wall_height, wall_length);
	glCallList(CUBE);
	glPopAttrib();
	glEndList();

	// room list
	glNewList(ROOM, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	setColor(BLUE);
	glTranslatef(-wall_length, 0, 0);
	glScalef(0.4, 1, 1);
	glCallList(WALL);
	glPopMatrix();
		//4 walls
	glPushMatrix();
	setColor(BLUE);
	glTranslatef(wall_length, 0, 0);
	glScalef(0.4, 1, 1);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(0, 0, wall_length * 0.5);
	glRotatef(90, 0, 1, 0);
	glScalef(0.4, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(0, 0, -wall_length * 0.5);
	glRotatef(90, 0, 1, 0);
	glScalef(0.4, 1, 2);
	glCallList(WALL);
	glPopMatrix();
		// floor
	glPushMatrix();
	setColor(GRAY);
	glTranslatef(0, -3.0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(floor_scaleX, floor_scaleY, floor_scaleZ);
	glCallList(WALL);
	glPopMatrix();

	glPopAttrib();
	glEndList();// end room list

	// table and chairs
	glNewList(TABLE_CHAIRS, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	setColor(RED);
	glPushMatrix();
	glutSolidCube(3.0);
	glTranslatef(-3.0, 0.0, 0.0);
	glutSolidCube(1.0);
	glTranslatef(6.0, 0.0, 0.0);
	glutSolidCube(1.0);
	glTranslatef(-3.0, 0.0, -3.0);
	glutSolidCube(1.0);
	glTranslatef(0.0, 0.0, 6.0);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// Mirror
	glNewList(MIRROR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glTranslatef(19.5, 0, 0);
	glScalef(0.1f, 0.8, 1);
	glutSolidCube(5.0);
	glPopAttrib();
	glPopMatrix();
	glEndList();

	glNewList(FAN, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	setColor(VIOLET);
	glTranslatef(0, wall_height / 2, 0);
	glutSolidSphere(fan_radius, fan_slices, fan_stacks);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, wall_height / 2, 0);
	glScalef(1, 0.1, 1);
	glutSolidCube(2.0);
	glTranslatef(-4, 0, 0);
	glutSolidCube(2.0);
	glTranslatef(2, 0, 2);
	glutSolidCube(2.0);
	glTranslatef(0, 0, -4);
	glutSolidCube(2.0);
	glPopMatrix();
	glPopAttrib();
	glEndList();

}

void setColor(GLint colorID)
{
	glColor3fv(current_color[colorID]);

}