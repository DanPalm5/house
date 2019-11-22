typedef struct MaterialType {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} MaterialType;
						


// Material types
MaterialType brass = { {0.33f,0.22f,0.03f,1.0f},
					  {0.78f,0.57f,0.11f,1.0f},
					  {0.99f,0.91f,0.81f,1.0f},
					   27.8f };

MaterialType lime = { {	0.0215,	0.1745,	0.0215},
					{0.07568, 0.61424, 0.07568},
					{0.633, 0.727811, 0.633},
										32 };


MaterialType ruby = { {0.1745,0.01175, 0.01175},
					{0.61424, 0.04136, 0.04136},
					{0.727811, 0.626959, 0.626959},
											32 };

MaterialType blue = { {0.1, 0.1, 0.3},
					{0.7, 0.7, 0.7},
					{0.50,0.50, 0.50},
								32 };
void set_material(GLenum face, MaterialType *material)
{
	glMaterialfv(face,GL_AMBIENT,material->ambient);
	glMaterialfv(face,GL_DIFFUSE,material->diffuse);
	glMaterialfv(face,GL_SPECULAR,material->specular);
	glMaterialf(face,GL_SHININESS,material->shininess);
}
