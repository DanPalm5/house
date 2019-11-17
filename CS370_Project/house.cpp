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
void load_image();
bool load_textures();


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

	// Set initial ambient light
	GLfloat background[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	set_AmbientLight(background);

	// Load shader programs
	defaultShaderProg = load_shaders(defaultVertexFile,defaultFragmentFile);
	lightShaderProg = load_shaders(lightVertexFile, lightFragmentFile);
	textureShaderProg = load_shaders(texVertexFile, texFragmentFile);
	numLights_param = glGetUniformLocation(lightShaderProg, "numLights");
	texSampler = glGetUniformLocation(textureShaderProg, "texMap");
	glUseProgram(defaultShaderProg);

	// Load textures
	if (!load_textures())
	{
		exit(0);
	}

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
	//UseProgram(textureShaderProg);
	// draw room
	glPushMatrix();
	glCallList(ROOM);
	glPopMatrix();
	
	// table and chairs
	setColor(RED);
	glPushMatrix();
	glTranslatef(-7.0f, -wall_height+1, 7);
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
	glCallList(DOOR);
	glPopMatrix();

	// fan
	glPushMatrix();
	glCallList(FAN);
	glPopMatrix();
	
	// fireplace
	glPushMatrix();
	glCallList(FIREPLACE);
	glPopMatrix();

	// window
	glPushMatrix();
	glCallList(WINDOW);
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
		if (key == 'D' || key == 'd') {
			camera_theta += camera_dtheta;
			if(camera_theta > CAMERA_LR_LIMIT) {
				camera_theta -= CAMERA_LR_LIMIT;
			}
			at_fp[X] = (eye_fp[X] + cos(camera_theta));
			at_fp[Z] = (eye_fp[Z] + sin(camera_theta));
		}
		// d to pivot right
		if (key == 'A' || key == 'a') {
			camera_theta -= camera_dtheta;
			if (camera_theta < -CAMERA_LR_LIMIT) {
				camera_theta += CAMERA_LR_LIMIT;
			}
			at_fp[X] = (eye_fp[X] + cos(camera_theta));
			at_fp[Z] = (eye_fp[Z] + sin(camera_theta));

		}
		// z to look up
		if (key == 'Z' || key == 'z') {
			camera_y_theta += camera_y_dtheta;
			if (camera_y_theta > CAMERA_LIMIT) {
				camera_y_theta = CAMERA_LIMIT;
			}
			at_fp[Y] = (eye_fp[Y] + sin(camera_y_theta));
		}

		// x to look down
		if (key == 'X' || key == 'x') {
			camera_y_theta -= camera_y_dtheta;
			if (camera_y_theta < -CAMERA_LIMIT) {
				camera_y_theta = -CAMERA_LIMIT;
			}
			at_fp[Y] = (eye_fp[Y] + sin(camera_y_theta));
		
		}
	}

		// <esc> quits
	if (key == 27)
	{
		exit(0);
	}

	if (key == 'p' || key == 'P') {
		projectionType = !projectionType;
		if (FIRSTPERSON) {
			camera_theta += camera_dtheta;
			if (camera_theta > 360.0f) {
				camera_theta -= 360.0f;
			}
			at_fp[X] = (eye_fp[X] + cos(camera_theta));
			at_fp[Z] = (eye_fp[Z] + sin(camera_theta));
		}
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

	// floor and ceiling list

	glNewList(FLOOR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[FLOOR_TEXTURE]);
	glScalef(1, wall_height, wall_length);
	texturecube();
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// wall list
	glNewList(WALL, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[WALL_TEXTURE]);
	glScalef(1, wall_height, wall_length);
	texturecube();
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// room list
	glNewList(ROOM, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);
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
	glCallList(FLOOR);
	glPopMatrix();

	// ceiling
	glPushMatrix();
	setColor(GRAY);
	glTranslatef(0, wall_height, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(floor_scaleX, floor_scaleY, floor_scaleZ);
	//glCallList(FLOOR;
	glPopMatrix();

	glPopAttrib();
	glEndList();// end room list

	// table and chairs
	glNewList(TABLE_CHAIRS, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);
	glPushMatrix();
		// table
	glTranslatef(CHAIR_TO_CHAIR_DIST / 4, 0, -CHAIR_TO_CHAIR_DIST / 4);
	glScalef(4, 1, 4);
	glCallList(FULL_CHAIR);
	glPopMatrix();
		// 4 chairs
	glPushMatrix();
	glTranslatef(-CHAIR_TO_CHAIR_DIST, 0.0, 0.0);
	glCallList(FULL_CHAIR);
	glTranslatef(CHAIR_TO_CHAIR_DIST*2, 0.0, 0.0);
	glCallList(FULL_CHAIR);
	glTranslatef(-CHAIR_TO_CHAIR_DIST, 0.0, -CHAIR_TO_CHAIR_DIST);
	glCallList(FULL_CHAIR);
	glTranslatef(0.0, 0.0, CHAIR_TO_CHAIR_DIST*2);
	glCallList(FULL_CHAIR);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// Mirror
	glNewList(MIRROR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);
	glPushMatrix();
	glTranslatef((wall_length*2)-1.0, 0, -5.0f);
	glScalef(0.1f, 4, 4);
	glCallList(CUBE);
	glPopAttrib();
	glPopMatrix();
	glEndList();

	// fan
	glNewList(FAN, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);

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

	// fireplace list
	glNewList(FIREPLACE, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);

	glPushMatrix();
	setColor(BROWN);
	glTranslatef((-wall_length * 2) + 1.05, (-wall_height / 2) - 0.75, 0);
	glScalef(1, 2.75, 3.25);
	glCallList(CUBE);
	glPopMatrix();


	glPushMatrix();
	setColor(RED);
	glTranslatef((-wall_length * 2) + 1.0, -wall_height / 2, 0);
	glScalef(1, 3.5, 4);
	glCallList(CUBE);
	glPopMatrix();
	glPopAttrib();
	glEndList();  // end fireplace list

	// door list
	glNewList(DOOR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);

	glPushMatrix();
	glTranslatef(0, -wall_height / 2, (-wall_length * 2));
	glScalef(2, 4, 1.1);
	glCallList(CUBE);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// window list
	glNewList(WINDOW, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);

	glPushMatrix();
	setColor(GLASS);
	glTranslatef(0, -wall_height / 3, (wall_length * 2));
	glScalef(3, 3, 1.1);
	glCallList(CUBE);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// chair leg
	glNewList(CHAIR_LEG, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);

	glPushMatrix();
	setColor(BROWN);
	glScalef(CHAIR_LEG_SCALEX, CHAIR_LEG_SCALEY, CHAIR_LEG_SCALEZ);
	glCallList(CUBE);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// chair seat
	glNewList(CHAIR_SEAT, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);

	glPushMatrix();
	setColor(BROWN);
	glTranslatef(0, 0.25, 0);
	glScalef(CHAIR_SEAT_SCALE, CHAIR_SEAT_SCALE_Y, CHAIR_SEAT_SCALE);
	glCallList(CUBE);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// complete chair list
	glNewList(FULL_CHAIR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);

	glPushMatrix();
	glCallList(CHAIR_LEG);
	glTranslatef(-CHAIR_WIDTH, 0, 0);
	glCallList(CHAIR_LEG);
	glTranslatef(0, 0, CHAIR_WIDTH);
	glCallList(CHAIR_LEG);
	glTranslatef(CHAIR_WIDTH, 0, 0);
	glCallList(CHAIR_LEG);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-CHAIR_WIDTH / 2, CHAIR_LEG_TO_SEAT_HEIGHT, CHAIR_WIDTH / 2);
	glCallList(CHAIR_SEAT);
	glPopMatrix();
	glPopAttrib();
	glEndList();
}

void setColor(GLint colorID)
{
	glColor3fv(current_color[colorID]);
}

void load_image() 
{

}

bool load_textures() 
{
	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		// TODO: Load images
		tex_ids[i] = SOIL_load_OGL_texture(texture_files[i], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

		// Set texture properties if successfully loaded
		if (tex_ids[i] != 0)
		{
			// TODO: Set scaling filters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			// TODO: Set wrapping modes
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		// Otherwise texture failed to load
		else
		{
			return false;
		}
	}
	return true;
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

//Routine to draw quadrilateral face
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