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
#define ART 5
#define FAN 6
#define FIREPLACE 7
#define DOOR 8
#define WINDOW 9
#define CHAIR_LEG 10
#define CHAIR_SEAT 11
#define FULL_CHAIR 12
#define FLOOR 13
#define TREE 14
#define TABLE_TOP 15
#define CEILING 16
#define MIRROR 17

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
#define GLASS 11

// Vertex colors
GLfloat current_color[][3] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f, 0.5f}, { 1.0f, 1.0f, 0.0f } ,{0.69f, 0.13f, 0.13f}, {1.0f,0.35f,0.39f},{0.0f, 1.0f, 1.0f},
{0.93f,0.51f , 0.93f}, {0.0f, 0.0f, 0.0f}, {0.82f, 0.41f, 0.11f}, {0.658f, 0.8f, 0.843f} };
GLint color = RED;

// cube vertices
GLfloat cube[][3] = { { -1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, 1.0f },
{ -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, -1.0f },
{ 1.0f, 1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f } };


// Global camera vectors
GLfloat eye[3] = { 1.0f,1.0f,1.0f };
GLfloat at[3] = { 0.0f,0.0f,0.0f };
GLfloat up[3] = { 0.0f,1.0f,0.0f };

GLfloat eye_fp[3] = { 0, -3, 0 };
GLfloat at_fp[3] = { 0, -3, 0 };
GLfloat up_fp[3] = { 0, 1, 0 };


// Global spherical coord values
GLfloat azimuth = 45.0f;
GLfloat daz = 2.0f;
GLfloat elevation = 45.0f;
GLfloat del = 2.0f;
GLfloat radius = 4.0f;

// first person camera variables
GLfloat camera_move_step = 0.20f;
GLfloat camera_pivot_step = camera_move_step;
GLfloat camera_theta = 0.0f *DEG2RAD;
GLfloat camera_dtheta = 2.0f * DEG2RAD;
GLfloat camera_y_theta = 0.0f * DEG2RAD;
GLfloat camera_y_dtheta = 1.0f *DEG2RAD;
#define CAMERA_LIMIT 45.0f*DEG2RAD
#define CAMERA_LR_LIMIT 360.0f*DEG2RAD
#define EYE_LIMIT wall_length*1.65

// House variables
GLfloat wall_length = 10.0f;
GLfloat wall_height = 7.5f;
GLfloat floor_scaleX = wall_length*2;
GLfloat floor_scaleY = 0.01;
GLfloat floor_scaleZ = wall_length/5;


// Tree variables
GLfloat tree_offset = wall_length*1.5;
GLfloat tree_base = 3.75;
GLfloat tree_height = 15;
GLfloat tree_slices = 500;
GLfloat tree_stacks = 500;
GLfloat tree_cover_inner_rad = 0.1f;
GLfloat tree_cover_outer_rad = 4.5f;
GLfloat tree_cover_height = 0.2f;

GLfloat stump_radius = 1;
GLfloat stump_height = 3;
GLfloat stump_slices = 300;
GLfloat stump_stacks = 300;

// fan variables
GLfloat fan_radius = 2;
GLfloat fan_slices = 20;
GLfloat fan_stacks = 20;

// chair variables
#define CHAIR_LEG_SCALEX 0.20f
#define CHAIR_LEG_SCALEY 1.0f
#define CHAIR_LEG_SCALEZ 0.20f
#define CHAIR_WIDTH 1.25f
#define CHAIR_LEG_TO_SEAT_HEIGHT 0.75f
#define CHAIR_SEAT_SCALE 1
#define CHAIR_SEAT_SCALE_Y 0.2

#define CHAIR_TO_CHAIR_DIST 7.0f

// textures
// texture constants
#define ENVIRONMENT 0
#define FLOOR_TEXTURE 1
#define DOOR_TEXTURE 2
#define STOOL_TEXTURE 3
#define STOOL_LEG_TEXTURE 4
#define FIRE 5
#define MANTLE 6
#define ARTWORK 7
#define TREE_TOP_TEXTURE 8
#define TREE_STUMP_TEXTURE 9
#define CEILING_TEXTURE 10
#define TREE_COVER_TEXTURE 11
#define PRESENT_ONE 12
#define PRESENT_TWO 13
#define PRESENT_THREE 14
#define WALL_TEXTURE 15
#define NUM_TEXTURES 16

	// texture array
	GLuint tex_ids[NUM_TEXTURES];

	// Texture files
	char texture_files[NUM_TEXTURES][20] = { {"blank.bmp"}, {"wood_floor_tex.jpg"} , {"door_texture.jpg"},
											{"stool_texture.jpg"}, {"stool_leg.jpeg"}, {"fire_tex.jpg"}, 
											{"fireplace.png"}, {"work_of_art.jpg"}, {"tree_top_tex.jpg"}, 
											{"tree_stump_tex.jpg"} , {"ceiling_tex.jpg"}, {"tree_cover.jpg"},
											{"pres_tex_1.jpg"} , {"pres_tex_2.jpg"} , {"pres_tex_3.jpg"},
												{"wall_texture.png"} };


	// texture coordinates

GLfloat cube_tex[][2] = { {0,1}, {0, 0}, {1, 0}, {1,1},
							{1,0}, {1, 1}, {1, 0}, {0,0},
							{0,0}, {0, 1}, {1, 1}, {1,0},
							{1,0}, {1, 1}, {0, 0}, {0,0},
							{1,0}, {1, 1}, {0, 1}, {0,0},
							{0,0}, {1, 0}, {1, 1}, {0,1} };



//light1 (green spotlight) Parameters 
GLfloat light1_pos[] = {-6, wall_height/2.0f, 6.0f };
GLfloat light1_dir[] = { 0,-1,0};
GLfloat light1_cutoff = 40;
GLfloat light1_exp = 3;
