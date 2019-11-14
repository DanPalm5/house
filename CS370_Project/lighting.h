// Structure for lighting coefficients
struct LightType {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
};

// Lighting properties for basic white light
LightType white_light = {{0.0f,0.0f,0.0f,1.0f},
                         {1.0f,1.0f,1.0f,1.0f},
                         {1.0f,1.0f,1.0f,1.0f}};
					   
// Utility function to set directional light properties
void set_DirectionalLight(GLenum source, LightType *light, GLfloat direction[4])
{
	glLightfv(source,GL_AMBIENT,light->ambient);
	glLightfv(source,GL_DIFFUSE,light->diffuse);
	glLightfv(source,GL_SPECULAR,light->specular);
	// Only direction
	direction[3] = 0.0;
	glLightfv(source,GL_POSITION,direction);
}

// Utility function to set point light properties
void set_PointLight(GLenum source, LightType *light, GLfloat position[4])
{
	glLightfv(source,GL_AMBIENT,light->ambient);
	glLightfv(source,GL_DIFFUSE,light->diffuse);
	glLightfv(source,GL_SPECULAR,light->specular);
	// Only position
	position[3] = 1.0;
	glLightfv(source,GL_POSITION,position);
	glLightf(source,GL_SPOT_CUTOFF, 180.0);
	glLightf(source,GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(source,GL_LINEAR_ATTENUATION, 0.0);
	glLightf(source,GL_QUADRATIC_ATTENUATION, 0.0);
}

// Utility function to set (attenuated) point light properties
void set_PointLightAttenuated(GLenum source, LightType *light, GLfloat position[4], GLfloat attenuation[3])
{
	glLightfv(source,GL_AMBIENT,light->ambient);
	glLightfv(source,GL_DIFFUSE,light->diffuse);
	glLightfv(source,GL_SPECULAR,light->specular);
	// Only position
	position[3] = 1.0;
	glLightfv(source,GL_POSITION,position);
	glLightf(source,GL_SPOT_CUTOFF, 180.0);
	glLightf(source,GL_CONSTANT_ATTENUATION, attenuation[0]);
	glLightf(source,GL_LINEAR_ATTENUATION, attenuation[1]);
	glLightf(source,GL_QUADRATIC_ATTENUATION, attenuation[2]);
}

// Utility function to set spot light properties
void set_SpotLight(GLenum source, LightType *light, GLfloat position[4], GLfloat spot_direction[3], GLfloat spot_cutoff, GLfloat spot_exponent )
{
	glLightfv(source,GL_AMBIENT,light->ambient);
	glLightfv(source,GL_DIFFUSE,light->diffuse);
	glLightfv(source,GL_SPECULAR,light->specular);
	// Only position
	position[3] = 1.0;
	glLightfv(source,GL_POSITION,position);
	glLightfv(source,GL_SPOT_DIRECTION, spot_direction);
	glLightf(source, GL_SPOT_EXPONENT, spot_exponent);
	glLightf(source, GL_SPOT_CUTOFF, spot_cutoff);
	glLightf(source,GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(source,GL_LINEAR_ATTENUATION, 0.0);
	glLightf(source,GL_QUADRATIC_ATTENUATION, 0.0);
}

// Utility function to set (attenuated) spot light properties
void set_SpotLightAttenuated(GLenum source, LightType *light, GLfloat position[4], GLfloat spot_direction[3], GLfloat spot_cutoff, GLfloat spot_exponent, GLfloat attenuation[3] )
{
	glLightfv(source,GL_AMBIENT,light->ambient);
	glLightfv(source,GL_DIFFUSE,light->diffuse);
	glLightfv(source,GL_SPECULAR,light->specular);
	// Only position
	position[3] = 1.0;
	glLightfv(source,GL_POSITION,position);
	glLightfv(source,GL_SPOT_DIRECTION, spot_direction);
	glLightf(source, GL_SPOT_EXPONENT, spot_exponent);
	glLightf(source, GL_SPOT_CUTOFF, spot_cutoff);
	glLightf(source,GL_CONSTANT_ATTENUATION, attenuation[0]);
	glLightf(source,GL_LINEAR_ATTENUATION, attenuation[1]);
	glLightf(source,GL_QUADRATIC_ATTENUATION, attenuation[2]);
}

// Utility function to set ambient light
void set_AmbientLight(GLfloat color[4])
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,color);
}
