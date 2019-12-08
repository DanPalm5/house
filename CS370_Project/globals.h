// Projection states
#define ORTHOGRAPHIC 0
#define FIRSTPERSON 1
int projectionType = FIRSTPERSON;

// Shader file utility functions
#include "shaderutils.h"
#define X 0
#define Y 1
#define Z 2
#define RAD2DEG (180.0f/3.14159f)
#define DEG2RAD (3.14159f/180.0f)
#define LEMON_UNIT 0
#define LEMON_NORMAL_UNIT 1
#define LEMON_BUMP 0
#define LEMON_NORMAL_BUMP 1

#define APPLE_UNIT 0
#define APPLE_NORMAL_UNIT 1
#define APPLE_BUMP 0
#define APPLE_NORMAL_BUMP 1

#define ORANGE_UNIT 0
#define ORANGE_NORMAL_UNIT 1
#define ORANGE_BUMP 0
#define ORANGE_NORMAL_BUMP 1

#define FRUIT_UNIT 0
#define FRUIT_NORMAL_UNIT 1


// lists
#define CUBE 1
#define WALL 2
#define FOUR_WALLS 3
#define TABLE_CHAIRS 4
#define ART 5
#define FAN 6
#define FIREPLACE_MANTLE 7
#define DOOR 8
#define WINDOW 9
#define CHAIR_LEG 10
#define CHAIR_SEAT 11
#define FULL_CHAIR 12
#define FLOOR 13
#define TREE_BASE 14
#define TABLE_TOP 15
#define CEILING 16
#define TREE_TOP 17
#define STAR 18
#define DESK 19
#define WINDOW_PANE 20
#define WINDOW_BLINDS 21
#define SNOWGLOBE 22
#define SNOWGLOBE_BASE 23
#define FULL_WINDOW_PANE 24
#define TEAPOT_LIST 25
#define CUP 26
#define STEREO 27
#define FAN_BLADES 28
#define LEMON 29
#define FIREPLACE_FIRE 30
#define TREE_COVER 31
#define TREE_PRESENT_ONE 32
#define TREE_PRESENT_TWO 33
#define TREE_PRESENT_THREE 34
#define DESK_LAMP 35
#define SNOWGLOBE_SNOW 36
#define SNOWGLOBE_SCENE 37
#define STEREO_FRONT 38
#define FRUIT_BOWL 39


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
#define WHITE_SAND 12
#define CHOCOLATE_BROWN 13

// Vertex colors
GLfloat current_color[][3] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f, 0.5f}, { 1.0f, 1.0f, 0.0f } ,{0.69f, 0.13f, 0.13f}, {1.0f,0.35f,0.39f},{0.0f, 1.0f, 1.0f},
{0.93f,0.51f , 0.93f}, {0.0f, 0.0f, 0.0f}, {0.82f, 0.41f, 0.11f}, {0.658f, 0.8f, 0.843f}, {0.878f, 0.851f, 0.7801f} , {0.247f, 0.0f, 0.0588f} };
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
GLfloat tree_height = 13;
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
GLfloat fan_radius = 0.7;
GLfloat fan_slices = 20;
GLfloat fan_stacks = 20;
GLfloat fan_theta = 0.0f;
GLint spin_fan = 0;


// Snowglobe variables
GLfloat globe_bot_rad = 1.1f;
GLfloat globe_top_rad = 0.7f;
GLfloat globe_base_stacks = 100;
GLfloat globe_base_slices = 100;
GLfloat globe_height = 0.8f;

// teapot variables
#define POT_X TABLE_OFFSET - 1.0f
#define POT_Y (-wall_height/2.0f) 
#define POT_Z 10.0f

#define CUP_X TABLE_OFFSET
#define CUP_Y (-wall_height/2.0f) - 1.2f
#define CUP_Z 9.99f
#define CUP_HEIGHT 0.9f
#define CUP_RAD 0.25f

#define TEA_HEIGHT 0.6f
#define TEA_RAD 0.23f
// Animation variables
GLint time = 0;
GLint lasttime = 0;
GLint fps = 60;
GLfloat rpm = 20.0f;
GLfloat tree_theta = 0.0f;
GLint spin_tree = 0;
GLint animate_blinds = 0;
GLfloat blinds_shift = 0.0f;
GLfloat scale_y_theta = 1.0f;
GLfloat snowglobe_theta = 0.0f;
GLint animate_globe = 0;
GLint animate_teapot = 0;
GLint teapot_dir = 1;
GLfloat teapot_theta = 0.0f;
#define BLINDS_MAX_SHIFT 1.25f

// chair variables
#define CHAIR_LEG_SCALEX 0.20f
#define CHAIR_LEG_SCALEY 1.0f
#define CHAIR_LEG_SCALEZ 0.20f
#define CHAIR_WIDTH 1.25f
#define CHAIR_LEG_TO_SEAT_HEIGHT 0.75f
#define CHAIR_SEAT_SCALE 1
#define CHAIR_SEAT_SCALE_Y 0.2
#define CHAIR_TO_CHAIR_DIST 7.0f

#define TABLE_OFFSET -7.0f


// desk variables
#define DESK_OFFSET -wall_length*1.5f
GLint div_level = 8;


// bump mapped spheres
bool use_bump_lemon = true;
bool use_bump_apple = true;
bool use_bump_orange = true;


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
#define WINDOW_PANE_TEX 16
#define WINDOW_BLINDS_TEXTURE 17
#define SNOWGLOBE_TEX 18
#define STEREO_TEX 19
#define FAN_LIGHT_TEX 20
#define FAN_BLADE_TEX 21
#define LEMON_NORMAL_MAP 22
#define LEMON_TEX 23
#define APPLE_NORMAL_MAP 24
#define APPLE_TEX 25
#define ORANGE_NORMAL_MAP 26
#define ORANGE_TEX 27

#define NUM_TEXTURES 28


	// texture array
	GLuint tex_ids[NUM_TEXTURES];

	// Texture files
	char texture_files[NUM_TEXTURES][30] = { {"blank.bmp"}, {"wood_floor_tex.jpg"} , {"door_texture.jpg"},
											{"stool_texture.jpg"}, {"stool_leg.jpeg"}, {"fire_tex.jpg"}, 
											{"fireplace.png"}, {"work_of_art.jpg"}, {"tree_top_tex.jpg"}, 
											{"tree_stump_tex.jpg"} , {"ceiling_tex.jpg"}, {"tree_cover.jpg"},
											{"pres_tex_1.jpg"} , {"pres_tex_2.jpg"} , {"pres_tex_3.jpg"},
											{"wall_texture.png"}, {"window_tex.jpg"} , {"window_blinds.jpg"}, 
											{"snowglobe_tex.jpg"}, {"stereo_tex.jpg"} , {"fan_light.png"} , 
											{"fan_blade.jpeg"}, {"NormalMap.png"}, {"lemon_peel_tex.jpg"},
											{"apple_normal.png"}, {"apple_texture.jpg"} , {"orange_normal.png"}, 
																							{"orange_tex.jpg"} };

	// texture coordinates

GLfloat cube_tex[][2] = { {0,1}, {0, 0}, {1, 0}, {1,1},
							{1,0}, {1, 1}, {1, 0}, {0,0},
							{0,0}, {0, 1}, {1, 1}, {1,0},
							{1,0}, {1, 1}, {0, 0}, {0,0},
							{1,0}, {1, 1}, {0, 1}, {0,0},
							{0,0}, {1, 0}, {1, 1}, {0,1} };



//light1 (white spotlight) Parameters 
GLfloat light1_pos[] = { DESK_OFFSET - 0.5f , wall_height / 5.0f, DESK_OFFSET + 0.5f };
GLfloat light1_dir[] = { 0,-1,0};
GLfloat light1_cutoff = 30;
GLfloat light1_exp = 1;

// light2 (lime spotlight) parameters
GLfloat light2_pos[]{ POT_X, POT_Y + 5.0f, POT_Z };
GLfloat light2_dir[] = { 0, -1, 0 };
GLfloat light2_cutoff = 30;
GLfloat light2_exp = 1;
