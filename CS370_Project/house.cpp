// CS370 - Fall 2019
// Final Project
//Daniel Palmieri T/TR 12:30-1:45
//Room currently contains basic geometry for a fireplace, fan, table with chairs, door, mirror, christmas tree, and window.
// to toggle projection modes, use P.
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
void texturecube();
void texquad(GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat v4[], GLfloat t1[], GLfloat t2[], GLfloat t3[], GLfloat t4[]);

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

	if (projectionType == ORTHOGRAPHIC) {
		glOrtho(-25.0f * xratio, 25.0f * xratio, -25.0f * yratio, 25.0f * yratio, -25.0f, 25.0f);

		// Set modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye[X], eye[Y], eye[Z], at[X], at[Y], at[Z], up[X], up[Y], up[Z]);
	}
	else if (projectionType == FIRSTPERSON) {
		gluPerspective(45.0f, 1.0f, 0.5f, 50.0f);
		// Set modelview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// gluLookAt(first person coordinates)
		gluLookAt(eye_fp[X], eye_fp[Y], eye_fp[Z], at_fp[X], at_fp[Y], at_fp[Z], up_fp[X], up_fp[Y], up_fp[Z]);
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
	glTranslatef(-11.0f, 0, 0);
	glCallList(TABLE_CHAIRS);
	glPopMatrix();

	// mirror
	glPushMatrix();
	glCallList(MIRROR);
	glPopMatrix();

	// Christmas tree
	glPushMatrix();
	setColor(GREEN);
	glTranslatef(tree_offset, -wall_height/2 , 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.75, 0.75, 0.75);
	glutSolidCone(tree_base, tree_height, tree_slices, tree_stacks);
	glPopMatrix();

	glPushMatrix();
	setColor(BROWN);
	GLUquadricObj* tree_stump;
	tree_stump = gluNewQuadric();
	gluQuadricDrawStyle(tree_stump, GLU_LINE);
	glTranslatef(tree_offset, -wall_height, 0);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(tree_stump, stump_radius,stump_radius, stump_height, stump_slices, stump_stacks);
	glPopMatrix();

	// door
	glPushMatrix();
	glTranslatef(0, -wall_height/2, (-wall_length*2));
	glScalef(2, 4, 1.1);
	glCallList(CUBE);
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
	glCallList(CUBE);
	glPopMatrix();
	

	glPushMatrix();
	setColor(RED);
	glTranslatef(-wall_length + 1.0 ,-0.5, 0);
	glScalef(0.4, 1, 1.5);
	glCallList(CUBE);
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

		// Adjust elevation angle
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
		

		// Compute cartesian camera position
		eye[X] = (GLfloat)(radius * sin(azimuth * DEG2RAD) * sin(elevation * DEG2RAD));
		eye[Y] = (GLfloat)(radius * cos(elevation * DEG2RAD));
		eye[Z] = (GLfloat)(radius * cos(azimuth * DEG2RAD) * sin(elevation * DEG2RAD));
	}
	else if (projectionType == FIRSTPERSON) {
		// w to move forwards
		if (key == 'W' || key == 'w') {
			eye_fp[X] -= (eye_fp[X] - at_fp[X]) * camera_move_step;
			eye_fp[Z] -= (eye_fp[Z] - at_fp[Z]) * camera_move_step;

			at_fp[X] = (eye_fp[X] + cos(camera_theta));
			at_fp[Z] = (eye_fp[Z] + sin(camera_theta));
		}
		// s to move backwards
		if (key == 'S' || key == 's') {
			eye_fp[X] += (eye_fp[X] - at_fp[X]) * camera_move_step;
			eye_fp[Z] += (eye_fp[Z] - at_fp[Z]) * camera_move_step;

			at_fp[X] = (eye_fp[X] + cos(camera_theta));
			at_fp[Z] = (eye_fp[Z] + sin(camera_theta));
		}
		// a to pivot left
		if (key == 'A' || key == 'a') {
			camera_theta += camera_dtheta;
			if (camera_theta > 360.0f) {
				camera_theta -= 360.0f;
			}
			at_fp[X] = (eye_fp[X] + cos(camera_theta));
			at_fp[Z] = (eye_fp[Z] + sin(camera_theta));
		}
		// d to pivot right
		if (key == 'D' || key == 'd') {
			camera_theta -= camera_dtheta;
			if (camera_theta < -360.0f) {
				camera_theta += 360.0f;
			}
			at_fp[X] = (eye_fp[X] + cos(camera_theta));
			at_fp[Z] = (eye_fp[Z] + sin(camera_theta));
			// z to look up
			if (key == 'Z' || key == 'z') {

			}

			// x to look down
			if (key == 'X' || key == 'x') {

			}


		}
	}

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
	texturecube();
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
	
		// 4 walls
	glPushMatrix();
	setColor(BLUE);
	glTranslatef(-wall_length*2, 0, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(wall_length*2, 0, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(0, 0, wall_length*2);
	glRotatef(90, 0, 1, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(0, 0, -wall_length*2);
	glRotatef(90, 0, 1, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();
		
	// floor
	glPushMatrix();
	setColor(GRAY);
	glTranslatef(0, -wall_height, 0);
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
	glTranslatef((wall_length*2)-1.0, 0, 0);
	glScalef(0.1f, 4, 4);
	glCallList(CUBE);
	glPopAttrib();
	glPopMatrix();
	glEndList();

	// fan
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

// Routine to draw textured cube
void texturecube()
{
	// Top face
	texquad(cube[4], cube[7], cube[6], cube[5], cube_tex[0], cube_tex[1], cube_tex[2], cube_tex[3]);

	// Bottom face
	texquad(cube[0], cube[1], cube[2], cube[3], cube_tex[4], cube_tex[5], cube_tex[6], cube_tex[7]);

	// Left face
	texquad(cube[2], cube[6], cube[7], cube[3], cube_tex[8], cube_tex[9], cube_tex[10], cube_tex[11]);

	// Right face
	texquad(cube[0], cube[4], cube[5], cube[1], cube_tex[12], cube_tex[13], cube_tex[14], cube_tex[15]);

	// Front face
	texquad(cube[1], cube[5], cube[6], cube[2], cube_tex[16], cube_tex[17], cube_tex[18], cube_tex[19]);

	// Back face
	texquad(cube[0], cube[3], cube[7], cube[4], cube_tex[20], cube_tex[21], cube_tex[22], cube_tex[23]);

}

// Routine to draw quadrilateral face
void texquad(GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat v4[], GLfloat t1[], GLfloat t2[], GLfloat t3[], GLfloat t4[])
{
	// Draw face 
	glBegin(GL_POLYGON);
	glTexCoord2fv(t1);
	glVertex3fv(v1);
	glTexCoord2fv(t2);
	glVertex3fv(v2);
	glTexCoord2fv(t3);
	glVertex3fv(v3);
	glTexCoord2fv(t4);
	glVertex3fv(v4);
	glEnd();
}