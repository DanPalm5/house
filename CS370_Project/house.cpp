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
#include "vectorops.h"


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
GLint numLights = 2;
GLint texSampler;

GLenum lights[4] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3 };


// create quadric object
GLUquadricObj* tree_stump;
GLUquadricObj* tree_top;
GLUquadricObj* tree_cover;



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
void create_Mirror();
void render_Mirror();
void colorcube();
void div_quad(GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat v4[], int n);
void rquad(GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat v4[]);


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

	// Load textures
	if (!load_textures())
	{
		exit(0);
	}

	// create quadrics with texture coords
	tree_stump = gluNewQuadric();
	gluQuadricDrawStyle(tree_stump, GLU_FILL);
	gluQuadricTexture(tree_stump, GL_TRUE);

	tree_top = gluNewQuadric();
	gluQuadricDrawStyle(tree_top, GLU_FILL);
	gluQuadricTexture(tree_top, GL_TRUE);

	tree_cover = gluNewQuadric();
	gluQuadricDrawStyle(tree_cover, GLU_FILL);
	gluQuadricTexture(tree_cover, GL_TRUE);


	//create lists
	create_lists();

	// Begin event loop
	glutMainLoop();
	return 0;
}

// Display callback
void display()
{	// pass 1
	create_Mirror();

	// Reset background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set initial ambient light
	GLfloat background[] = { 0.2f, 0.2f, 1.0f, 1.0f };
	set_AmbientLight(background);


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

	// render mirror
	render_Mirror();

	// Flush buffer
	glFlush();
	
	// Swap buffers
	glutSwapBuffers(); 
}

// Scene render function
void render_Scene()
{

	set_SpotLight(GL_LIGHT1, &white_light , light1_pos, light1_dir, light1_cutoff, light1_exp);
	

	// draw room
	glPushMatrix();
	glCallList(ROOM);
	glPopMatrix();
	
	// table and chairs
	glPushMatrix();
	setColor(RED);
	glTranslatef(-7.0f, -wall_height+1, 7);
	glCallList(TABLE_CHAIRS);
	glCallList(TABLE_TOP);
	glPopMatrix();  

	// mirror
	glPushMatrix();
	glCallList(ART);
	glPopMatrix();

	// Christmas tree
			// base
		glPushMatrix();
		glCallList(TREE);
		glPopMatrix();

			// top
		glPushMatrix();
		glTranslatef(tree_offset, (-wall_height / 2) - 1, tree_offset * 0.9);
		glRotatef(-90, 1, 0, 0);
		glRotatef(tree_theta, 0, 0, 1);
		glCallList(TREE_TOP);
		glPopMatrix();

			//star on top
		glPushMatrix();
		glTranslatef(tree_offset, 5.5, tree_offset * 0.9);
		glRotatef(tree_theta, 0, 1, 0);
		glCallList(STAR);
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

		// window pane
			// top
		glPushMatrix();
		glTranslatef(0, 0.25f, (wall_length * 2) - 0.25f);
		glScalef(1.2, 1, 1);
		glCallList(WINDOW_PANE);
		glPopMatrix();
			// bottom
		glPushMatrix();
		glTranslatef(0, -4.0f, (wall_length * 2) - 0.25f);
		glScalef(1.2, 1, 1);
		glCallList(WINDOW_PANE);
		glPopMatrix();
			// right side
		glPushMatrix();
		glTranslatef(-2.15f, -2.0f, (wall_length * 2) - 0.25f);
		glRotatef(90, 0, 0, 1);
		glScalef(1.05f, 1.05f, 1);
		glCallList(WINDOW_PANE);
		glPopMatrix();
			// left side 
		glPushMatrix();
		glTranslatef(2.15f, -2.0f, (wall_length * 2) - 0.25f);
		glRotatef(90, 0, 0, 1);
		glScalef(1.05f, 1.05f, 1);
		glCallList(WINDOW_PANE);
		glPopMatrix();
			// middle column
		glPushMatrix();
		glTranslatef(0, -2.0f, (wall_length * 2) - 0.25f);
		glRotatef(90, 0, 0, 1);
		glScalef(1.05f, 1.05f, 1);
		glCallList(WINDOW_PANE);
		glPopMatrix();
			// middle row
		glPushMatrix();
		glTranslatef(0, -2.0f, (wall_length * 2) - 0.25f);
		glScalef(1.05f, 1.05f, 1);
		glCallList(WINDOW_PANE);
		glPopMatrix();

			// window blinds
		glPushMatrix();
		glTranslatef(0, (-wall_height / 4.0f)+(blinds_shift+(1.0f-scale_y_theta)), (wall_length * 2) - 0.50f);
		glRotatef(180, 0, 1, 0);
		glScalef(2.25f, 2.25f *scale_y_theta, 1.1f);
		glCallList(WINDOW_BLINDS);
		glPopMatrix();

	// desk
	glPushMatrix();
	glUseProgram(lightShaderProg);
	glUniform1i(numLights_param, numLights);
	set_material(GL_FRONT_AND_BACK, &brass);
	glCallList(DESK);
	glPopMatrix();

	// desk lamp
		// rod
	glPushMatrix();
	glTranslatef(DESK_OFFSET-1, wall_height / 4.0f, DESK_OFFSET-0.5f);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(tree_top, 0.1, 0.1, wall_height, 50, 50);
	glPopMatrix();
		// cone bottom
	glPushMatrix();
	glTranslatef(DESK_OFFSET - 1, wall_height / 5.0f, DESK_OFFSET - 0.5f);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(tree_top, 0.5, 0.1, 1, 50, 50);
	glPopMatrix();

	// legs
	glPushMatrix();
	glTranslatef(DESK_OFFSET-2.5 , -wall_height + 1.5, DESK_OFFSET-3.5);
	glScalef(1, 1.75f, 1);
	glCallList(CHAIR_LEG);
	glTranslatef(0, 0, 6.5f);
	glCallList(CHAIR_LEG);
	glTranslatef(3.0f, 0, 0);
	glCallList(CHAIR_LEG);
	glTranslatef(0, 0, -6.5f);
	glCallList(CHAIR_LEG);
	glPopMatrix();

	// desk chair
	glPushMatrix();
	glTranslatef(DESK_OFFSET + 3, -wall_height+1, DESK_OFFSET - 0.5);
	glCallList(FULL_CHAIR);
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
		
			if (eye_fp[Z] < -EYE_LIMIT) {
				eye_fp[Z] = -EYE_LIMIT;
			}
			if (eye_fp[X] < -EYE_LIMIT) {
				eye_fp[X] = -EYE_LIMIT;
			}
			if (eye_fp[Z] > EYE_LIMIT) {
				eye_fp[Z] = EYE_LIMIT;
			}
			if (eye_fp[X] > EYE_LIMIT) {
				eye_fp[X] = EYE_LIMIT;
			}


			at_fp[X] = (eye_fp[X] + cos(camera_theta));
			at_fp[Z] = (eye_fp[Z] + sin(camera_theta));



		}
		// s to move backwards
		if (key == 'S' || key == 's') {
			eye_fp[X] += (eye_fp[X] - at_fp[X]) * camera_move_step;
			eye_fp[Z] += (eye_fp[Z] - at_fp[Z]) * camera_move_step;
			if (eye_fp[Z] > EYE_LIMIT) {
				eye_fp[Z] = EYE_LIMIT;
			}
			if (eye_fp[X] > EYE_LIMIT) {
				eye_fp[X] = EYE_LIMIT;
			}
			if (eye_fp[Z] < -EYE_LIMIT) {
				eye_fp[Z] = -EYE_LIMIT;
			}
			if (eye_fp[X] < -EYE_LIMIT) {
				eye_fp[X] = -EYE_LIMIT;
			}

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
		// animation of tree
		if (key == 'T' || key == 't') {
			spin_tree = !spin_tree;
		}

		// light toggle
		if (key == 'L' || key == 'l') {
			light1_dir[Y] *= -1;
		}

		// window blinds toggle
		if (key == 'O' || key == 'o') {
			animate_blinds = !animate_blinds;
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
	// Time-based Animations

	// Get total elapsed time
	time = glutGet(GLUT_ELAPSED_TIME);

	// Update if past desired interval
	if (time - lasttime > 1000.0f / fps) {
		// spin tree
		if (spin_tree) {
			tree_theta += 3.0f * rpm * (time - lasttime) / 1000.0f;
			if (tree_theta >= 360.0f) {
				tree_theta /= 360.0f;
			}
		}

		// animate blinds (scrunch up)
		if (animate_blinds) {
			blinds_shift += 0.1f * rpm * (time - lasttime) / 1000.0f;
			scale_y_theta -= 0.05f;
			if (scale_y_theta <= 0.05f) {
				scale_y_theta = 0.05f;
			}
			if (blinds_shift >= BLINDS_MAX_SHIFT) {
				blinds_shift = BLINDS_MAX_SHIFT;
			}

		}
		// animate blinds (move them back down)
		if (!animate_blinds) {
			blinds_shift -= 0.1f * rpm * (time - lasttime) / 1000.0f;
			if (blinds_shift <= 0) {
				blinds_shift = 0;
			}
			scale_y_theta += 0.05f;
			if (scale_y_theta >= 1.0f) {
				scale_y_theta = 1.0f;
			}
		
		}
		// Update lasttime (reset timer)
		lasttime = time;

		glutPostRedisplay();
	}
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


void setColor(GLint colorID)
{
	glColor3fv(current_color[colorID]);
}

void load_image() 
{

}

bool load_textures() 
{
	// Load environment map texture (NO MIPMAPPING)
	tex_ids[ENVIRONMENT] = SOIL_load_OGL_texture(texture_files[0], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	// TODO: Set environment map properties if successfully loaded
	if (tex_ids[ENVIRONMENT] != 0)
	{
		// Set scaling filters (no mipmap)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Set wrapping modes (clamped)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	}
	// Otherwise texture failed to load
	else
	{
		return false;
	}

	// Load object textures normally
	for (int i = 1; i < NUM_TEXTURES; i++)
	{
		// Load images
		tex_ids[i] = SOIL_load_OGL_texture(texture_files[i], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);


		// Set texture properties if successfully loaded
		if (tex_ids[i] != 0)
		{
			// Set scaling filters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			// Set wrapping modes
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

void create_Mirror() 
{
	
	// PASS 1 - Render reflected scene
	// Reset background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Set projection matrix for flat "mirror" camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 8.0f, -7.0f, 7.0f, 0.5f, 50.0f);

	// Set modelview matrix positioning "mirror" camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-wall_length * 2.0f + 3.25, 0.5, 12.0f, wall_length*2.0f - 3.25 , 0.5, wall_length, 0, 1, 0);

	// Render scene from mirror
	glPushMatrix();
		render_Scene();
	glPopMatrix();

	glFinish();

	// Copy scene to texture
	glBindTexture(GL_TEXTURE_2D, tex_ids[ENVIRONMENT]);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 512, 512, 0);
}

void render_Mirror()
{
	
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
		// Draw mirror surface
		glBindTexture(GL_TEXTURE_2D, tex_ids[ENVIRONMENT]);
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);//1, 0);
			glVertex3f((-wall_length * 2 + 1.25), -4, 15);
			glTexCoord2f(0, 1);//1, 1);
			glVertex3f((-wall_length * 2 + 1.25), 3, 15);
			glTexCoord2f(1, 1);//0, 1);
			glVertex3f((-wall_length * 2 + 1.25), 3, 8);
			glTexCoord2f(1, 0);//0, 0);
			glVertex3f((-wall_length*2  + 1.25), -4, 8);  // should be -wall_length*2 + 1.25 ,but is currently 5.25 to test to make sure image is not on other side
		glEnd();
	glPopMatrix();
	
	glPushMatrix();
		//Draw mirror frame
		glTranslatef(0, 0, -0.25);
		glBegin(GL_LINE_LOOP);
		glVertex3f(-wall_length * 2 + 1.30, -4.05f, 15.2f);
		glVertex3f(-wall_length * 2 + 1.30, 3.05f, 15.2f);
		glVertex3f(-wall_length * 2 + 1.30, 3.05f, 8.2f);
		glVertex3f(-wall_length * 2 + 1.30, -4.05f, 8.2f);
		glEnd();

		glPopMatrix();

}

void colorcube()
{
	// Top face
	div_quad(cube[4], cube[7], cube[6], cube[5], div_level);
	
	// Bottom face
	rquad(cube[0], cube[1], cube[2], cube[3]);

	// Left face
	rquad(cube[0], cube[3], cube[7], cube[4]);

	// Right face
	rquad(cube[1], cube[5], cube[6], cube[2]);

	// Front face
	rquad(cube[2], cube[6], cube[7], cube[3]);

	// Back face
	rquad(cube[0], cube[4], cube[5], cube[1]);
}

// Routine to perform recursive subdivision
void div_quad(GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat v4[], int n)
{
	GLfloat v1_prime[3], v2_prime[3], v3_prime[3], v4_prime[3], v5_prime[3];

	// Recurse until n = 0
	if (n > 0)
	{
		// Compute midpoints
		for (int i = 0; i < 3; i++)
		{
			v1_prime[i] = (v4[i] + v1[i]) / 2.0f;
			v2_prime[i] = (v1[i] + v2[i]) / 2.0f;
			v3_prime[i] = (v2[i] + v3[i]) / 2.0f;
			v4_prime[i] = (v3[i] + v4[i]) / 2.0f;
			v5_prime[i] = (v1[i] + v2[i] + v3[i] + v4[i]) / 4.0f;
		}

		// Subdivide polygon
		div_quad(v1, v2_prime, v5_prime, v1_prime, n - 1);
		div_quad(v2_prime, v2, v3_prime, v5_prime, n - 1);
		div_quad(v1_prime, v5_prime, v4_prime, v4, n - 1);
		div_quad(v5_prime, v3_prime, v3, v4_prime, n - 1);
	}
	else
	{
		// Otherwise render quad
		rquad(v1, v2, v3, v4);
	}
}

// Routine to draw quadrilateral face
void rquad(GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat v4[])
{
	GLfloat normal[3];
	// Compute normal via cross product
	cross(v1, v2, v4, normal);
	normalize(normal);

	// Set normal
	glNormal3fv(normal);

	// Draw face 
	glBegin(GL_POLYGON);
	glVertex3fv(v1);
	glVertex3fv(v2);
	glVertex3fv(v3);
	glVertex3fv(v4);
	glEnd();
}


void create_lists()
{
	// cube list
	glNewList(CUBE, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	texturecube();
	glPopAttrib();
	glEndList();

	// floor list
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

	// ceiling list
	glNewList(CEILING, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[CEILING_TEXTURE]);
	glRotatef(180, 1, 0, 0);
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
	
	// 4 walls
	glPushMatrix();
	setColor(BLUE);
	glTranslatef(-wall_length * 2, 0, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(wall_length * 2, 0, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(0, 0, wall_length * 2);
	glRotatef(90, 0, 1, 0);
	glScalef(1, 1, 2);
	glCallList(WALL);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(0, 0, -wall_length * 2);
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
	glTranslatef(0, wall_height, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(floor_scaleX, floor_scaleY, floor_scaleZ);
	glCallList(CEILING);
	glPopMatrix();
	glPopAttrib();
	glEndList();// end room list

	// table and chairs
	glNewList(TABLE_CHAIRS, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
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
	glTranslatef(CHAIR_TO_CHAIR_DIST * 2, 0.0, 0.0);
	glCallList(FULL_CHAIR);
	glTranslatef(-CHAIR_TO_CHAIR_DIST, 0.0, -CHAIR_TO_CHAIR_DIST);
	glCallList(FULL_CHAIR);
	glTranslatef(0.0, 0.0, CHAIR_TO_CHAIR_DIST * 2);
	glCallList(FULL_CHAIR);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// work of art
	glNewList(ART, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUniform1i(texSampler, 0);
	glUseProgram(textureShaderProg);
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, tex_ids[ARTWORK]);
	glTranslatef((wall_length * 2) - 1.0, 0, -5.0f);
	glScalef(0.1f, 4, 4);
	texturecube();
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
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);

	//actual fire
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[FIRE]);
	glTranslatef((-wall_length * 2) + 0.05, (-wall_height / 2) - 1.25, 0);
	glScalef(1, 2, 2.2);
	texturecube();
	glPopMatrix();

	//mantel
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, tex_ids[MANTLE]);

	setColor(RED);
	glTranslatef((-wall_length * 2) + 0.01, (-wall_height / 2) - 0.5, 0);
	glScalef(1, 3.5, 4);
	glCallList(CUBE);
	glDisable(GL_BLEND);
	glPopMatrix();
	glPopAttrib();
	glEndList();  // end fireplace list

	// door list
	glNewList(DOOR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[DOOR_TEXTURE]);
	glTranslatef(0, (-wall_height / 2) + 2.25, (-wall_length * 2) + 0.5);
	glScalef(5, 6, 0.7);
	texturecube();
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// window list
	glNewList(WINDOW, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
		//window pane texture
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glBindTexture(GL_TEXTURE_2D, tex_ids[WINDOW_PANE_TEX]);
	glTranslatef(0, -wall_height / 4.0f, (wall_length * 2));
	glRotatef(180, 0, 1, 0);
	glScalef(2.0f, 2.0f, 1.1f);
	glCallList(CUBE);
	glPopMatrix();
	glPopAttrib();
	glEndList();
		
	// pane instance object
	glNewList(WINDOW_PANE, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glUseProgram(defaultShaderProg);
	setColor(WHITE_SAND);
	glScalef(2.0f, 0.15f, 1.1f);
	glutSolidCube(2.0);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// window blinds
	glNewList(WINDOW_BLINDS, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[WINDOW_BLINDS_TEXTURE]);
	texturecube();
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// chair leg
	glNewList(CHAIR_LEG, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[STOOL_LEG_TEXTURE]);
	setColor(BROWN);
	glScalef(CHAIR_LEG_SCALEX, CHAIR_LEG_SCALEY, CHAIR_LEG_SCALEZ);
	glCallList(CUBE);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// chair seat
	glNewList(CHAIR_SEAT, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex_ids[STOOL_TEXTURE]);
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
	glPushMatrix();
	setColor(BROWN);
	glCallList(CHAIR_LEG);
	glTranslatef(-CHAIR_WIDTH, 0, 0);
	glCallList(CHAIR_LEG);
	glTranslatef(0, 0, CHAIR_WIDTH);
	glCallList(CHAIR_LEG);
	glTranslatef(CHAIR_WIDTH, 0, 0);
	glCallList(CHAIR_LEG);
	glPopMatrix();

	glPushMatrix();
	setColor(BROWN);
	glBindTexture(GL_TEXTURE_2D, tex_ids[STOOL_TEXTURE]);
	glTranslatef(-CHAIR_WIDTH / 2, CHAIR_LEG_TO_SEAT_HEIGHT, CHAIR_WIDTH / 2);
	glCallList(CHAIR_SEAT);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	// tree list (excludes top)
	glNewList(TREE, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);

	//base
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[TREE_STUMP_TEXTURE]);
	glTranslatef(tree_offset, -wall_height, tree_offset * 0.9);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.60, 0.60, 1);
	gluCylinder(tree_stump, stump_radius, stump_radius, stump_height, stump_slices, stump_stacks);
	glPopMatrix();


	// cover
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[TREE_COVER_TEXTURE]);
	glTranslatef(tree_offset, -wall_height + 0.15f, tree_offset * 0.9);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.75, 0.75, 0.75);
	gluDisk(tree_cover, tree_cover_inner_rad, tree_cover_outer_rad, tree_slices, tree_stacks);
	glDisable(GL_BLEND);
	glPopMatrix();


	// presents

		//1
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[PRESENT_ONE]);
	glTranslatef(tree_offset - 1, -wall_height + 0.15f, tree_offset * 0.6);
	glRotatef(45, 0, 1, 0);
	texturecube();
	glPopMatrix();
	//2
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[PRESENT_TWO]);
	glTranslatef(tree_offset - 2, -wall_height + 0.15f, tree_offset);
	glRotatef(45, 0, 1, 0);

	texturecube();
	glPopMatrix();

	//3
	glPushMatrix();
	glUseProgram(textureShaderProg);
	glUniform1i(texSampler, 0);
	glBindTexture(GL_TEXTURE_2D, tex_ids[PRESENT_THREE]);
	glTranslatef(tree_offset - 4, -wall_height + 0.15f, tree_offset * 0.7);
	texturecube();
	glPopMatrix();

	glPopAttrib();
	glEndList(); //end tree list (excludes top)


	// tree top list
	glNewList(TREE_TOP, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
		// top
		glPushMatrix();
		glUseProgram(textureShaderProg);
		glUniform1i(texSampler, 0);
		glBindTexture(GL_TEXTURE_2D, tex_ids[TREE_TOP_TEXTURE]);
		glScalef(0.75, 0.75, 0.75);
		gluCylinder(tree_top, tree_base, 0.1, tree_height, tree_slices, tree_stacks);
		glPopMatrix();
		glPopAttrib();
		glEndList();


		//star list
		glNewList(STAR, GL_COMPILE);
		glPushAttrib(GL_CURRENT_BIT);
		glPushMatrix();
		glUseProgram(defaultShaderProg);
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(0.5, 0.5, 0.5);
		glutSolidIcosahedron();
		glPopMatrix();
	glPopAttrib();
	glEndList();


	// mirror 
	glNewList(MIRROR, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glUseProgram(defaultShaderProg);
	glPushMatrix();

	glPopMatrix();
	glPopAttrib();
	glEndList();


	// desk list
	glNewList(DESK, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glTranslatef(DESK_OFFSET-1, -wall_height/1.75f, DESK_OFFSET);
	glScalef(2.0f, 0.2f, 4.0f);
	colorcube();
	glPopMatrix();
	glPopAttrib();
	glEndList();

}