#ifdef OSX
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <math.h>

#define PI 3.14159
#define DEG_TO_RAD (PI/180.0)
#define ANG_STEP 10.0
#define NUM_DIVS 50
#define CACHE_SIZE 240

// Modified from the glu source code for gluSphere() for a multi-textured unit sphere with normals
void mySphere2(bool use_bump, GLuint tangParam)
{
	GLint i, j;
	GLfloat sintheta[NUM_DIVS + 1];
	GLfloat costheta[NUM_DIVS + 1];
	GLfloat sinphi[NUM_DIVS + 1];
	GLfloat cosphi[NUM_DIVS + 1];
	GLfloat angle;
	GLfloat sintemp1 = 0.0, sintemp2 = 0.0;
	GLfloat costemp1 = 0.0, costemp2 = 0.0;
	GLint slices = NUM_DIVS;
	GLint stacks = NUM_DIVS;
	GLfloat x, y, z;
	GLfloat s, t;
	GLfloat tangent[3];

	for (i = 0; i < slices; i++) {
		angle = 2.0 * PI * i / slices;
		sintheta[i] = sin(angle);
		costheta[i] = cos(angle);
	}

	for (j = 0; j <= stacks; j++) {
		angle = PI / 2.0 - PI * j / stacks;
		sinphi[j] = sin(angle);
		cosphi[j] = cos(angle);
	}
	/* Make sure it comes to a point */
	cosphi[0] = 0;
	cosphi[stacks] = 0;
	sintheta[slices] = sintheta[0];
	costheta[slices] = costheta[0];

	for (j = 0; j < stacks; j++) {
		sintemp1 = sinphi[j];
		sintemp2 = sinphi[j + 1];
		costemp1 = cosphi[j];
		costemp2 = cosphi[j + 1];

		glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices; i++) {
			// Compute coordinates
			x = costheta[i] * costemp2;
			y = sintheta[i] * costemp2;
			z = sintemp2;
			s = 1 - (float)i / slices;
			t = 1 - (float)(j + 1) / stacks;
			tangent[0] = -costheta[i] * sintemp2;
			tangent[1] = -sintheta[i] * sintemp2;
			tangent[2] = costemp2;
			// Set vectors
			glNormal3f(x, y, z);
			glMultiTexCoord2f(GL_TEXTURE0, s, t);
			if (use_bump)
			{
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				// TODO: Set tangent vector in shader
				glVertexAttrib3fv(tangParam, tangent);
			}
			glVertex3f(x, y, z);

			// Compute coordinates
			x = costheta[i] * costemp1;
			y = sintheta[i] * costemp1;
			z = sintemp1;
			s = 1 - (float)i / slices;
			t = 1 - (float)j / stacks;
			tangent[0] = -costheta[i] * sintemp1;
			tangent[1] = -sintheta[i] * sintemp1;
			tangent[2] = costemp1;
			// Set vectors
			glNormal3f(x, y, z);
			glMultiTexCoord2f(GL_TEXTURE0, s, t);
			if (use_bump)
			{
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				// TODO: Set tangent vector in shader
				glVertexAttrib3fv(tangParam, tangent);
			}
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

// Modified from the glu source code for gluSphere() available in the distribution at www.mesa3D.com
//
void mySphere(GLdouble radius, GLint slices, GLint stacks, GLint drawStyle, GLint normals, GLint textureCoords)
{
	GLint i, j;
	GLfloat sinCache1a[CACHE_SIZE];
	GLfloat cosCache1a[CACHE_SIZE];
	GLfloat sinCache2a[CACHE_SIZE];
	GLfloat cosCache2a[CACHE_SIZE];
	GLfloat sinCache3a[CACHE_SIZE];
	GLfloat cosCache3a[CACHE_SIZE];
	GLfloat sinCache1b[CACHE_SIZE];
	GLfloat cosCache1b[CACHE_SIZE];
	GLfloat sinCache2b[CACHE_SIZE];
	GLfloat cosCache2b[CACHE_SIZE];
	GLfloat sinCache3b[CACHE_SIZE];
	GLfloat cosCache3b[CACHE_SIZE];
	GLfloat angle;
	GLfloat zLow, zHigh;
	GLfloat sintemp1 = 0.0, sintemp2 = 0.0, sintemp3 = 0.0, sintemp4 = 0.0;
	GLfloat costemp1 = 0.0, costemp2 = 0.0, costemp3 = 0.0, costemp4 = 0.0;
	GLboolean needCache2, needCache3;
	GLint start, finish;
	GLint orientation = GLU_OUTSIDE;

	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;
	if (stacks >= CACHE_SIZE) stacks = CACHE_SIZE - 1;
	if (slices < 2 || stacks < 1 || radius < 0.0) {
		return;
	}

	/* Cache is the vertex locations cache */
	/* Cache2 is the various normals at the vertices themselves */
	/* Cache3 is the various normals for the faces */
	needCache2 = needCache3 = GL_FALSE;

	if (normals == GLU_SMOOTH) {
		needCache2 = GL_TRUE;
	}

	if (normals == GLU_FLAT) {
		if (drawStyle != GLU_POINT) {
			needCache3 = GL_TRUE;
		}
		if (drawStyle == GLU_LINE) {
			needCache2 = GL_TRUE;
		}
	}

	for (i = 0; i < slices; i++) {
		angle = 2 * PI * i / slices;
		sinCache1a[i] = sin(angle);
		cosCache1a[i] = cos(angle);
		if (needCache2) {
			sinCache2a[i] = sinCache1a[i];
			cosCache2a[i] = cosCache1a[i];
		}
	}

	for (j = 0; j <= stacks; j++) {
		angle = PI * j / stacks;
		if (needCache2) {
			if (orientation == GLU_OUTSIDE) {
				sinCache2b[j] = sin(angle);
				cosCache2b[j] = cos(angle);
			}
			else {
				sinCache2b[j] = -sin(angle);
				cosCache2b[j] = -cos(angle);
			}
		}
		sinCache1b[j] = radius * sin(angle);
		cosCache1b[j] = radius * cos(angle);
	}
	/* Make sure it comes to a point */
	sinCache1b[0] = 0;
	sinCache1b[stacks] = 0;

	if (needCache3) {
		for (i = 0; i < slices; i++) {
			angle = 2 * PI * (i - 0.5) / slices;
			sinCache3a[i] = sin(angle);
			cosCache3a[i] = cos(angle);
		}
		for (j = 0; j <= stacks; j++) {
			angle = PI * (j - 0.5) / stacks;
			if (orientation == GLU_OUTSIDE) {
				sinCache3b[j] = sin(angle);
				cosCache3b[j] = cos(angle);
			}
			else {
				sinCache3b[j] = -sin(angle);
				cosCache3b[j] = -cos(angle);
			}
		}
	}

	sinCache1a[slices] = sinCache1a[0];
	cosCache1a[slices] = cosCache1a[0];
	if (needCache2) {
		sinCache2a[slices] = sinCache2a[0];
		cosCache2a[slices] = cosCache2a[0];
	}
	if (needCache3) {
		sinCache3a[slices] = sinCache3a[0];
		cosCache3a[slices] = cosCache3a[0];
	}

	switch (drawStyle) {
	case GLU_FILL:
		/* Do ends of sphere as TRIANGLE_FAN's (if not texturing)
		** We don't do it when texturing because we need to respecify the
		** texture coordinates of the apex for every adjacent vertex (because
		** it isn't a constant for that point)
		*/
		if (!(textureCoords)) {
			start = 1;
			finish = stacks - 1;

			/* Low end first (j == 0 iteration) */
			sintemp2 = sinCache1b[1];
			zHigh = cosCache1b[1];
			switch (normals) {
			case GLU_FLAT:
				sintemp3 = sinCache3b[1];
				costemp3 = cosCache3b[1];
				break;
			case GLU_SMOOTH:
				sintemp3 = sinCache2b[1];
				costemp3 = cosCache2b[1];
				glNormal3f(sinCache2a[0] * sinCache2b[0], cosCache2a[0] * sinCache2b[0], cosCache2b[0]);
				break;
			default:
				break;
			}
			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0.0, 0.0, radius);
			if (orientation == GLU_OUTSIDE) {
				for (i = slices; i >= 0; i--) {
					switch (normals) {
					case GLU_SMOOTH:
						glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
						break;
					case GLU_FLAT:
						if (i != slices) {
							glNormal3f(sinCache3a[i + 1] * sintemp3, cosCache3a[i + 1] * sintemp3, costemp3);
						}
						break;
					case GLU_NONE:
					default:
						break;
					}
					glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
				}
			}
			else {
				for (i = 0; i <= slices; i++) {
					switch (normals) {
					case GLU_SMOOTH:
						glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
						break;
					case GLU_FLAT:
						glNormal3f(sinCache3a[i] * sintemp3, cosCache3a[i] * sintemp3, costemp3);
						break;
					case GLU_NONE:
					default:
						break;
					}
					glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
				}
			}
			glEnd();

			/* High end next (j == stacks-1 iteration) */
			sintemp2 = sinCache1b[stacks - 1];
			zHigh = cosCache1b[stacks - 1];
			switch (normals) {
			case GLU_FLAT:
				sintemp3 = sinCache3b[stacks];
				costemp3 = cosCache3b[stacks];
				break;
			case GLU_SMOOTH:
				sintemp3 = sinCache2b[stacks - 1];
				costemp3 = cosCache2b[stacks - 1];
				glNormal3f(sinCache2a[stacks] * sinCache2b[stacks], cosCache2a[stacks] * sinCache2b[stacks], cosCache2b[stacks]);
				break;
			default:
				break;
			}
			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0.0, 0.0, -radius);
			if (orientation == GLU_OUTSIDE) {
				for (i = 0; i <= slices; i++) {
					switch (normals) {
					case GLU_SMOOTH:
						glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
						break;
					case GLU_FLAT:
						glNormal3f(sinCache3a[i] * sintemp3, cosCache3a[i] * sintemp3, costemp3);
						break;
					case GLU_NONE:
					default:
						break;
					}
					glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
				}
			}
			else {
				for (i = slices; i >= 0; i--) {
					switch (normals) {
					case GLU_SMOOTH:
						glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
						break;
					case GLU_FLAT:
						if (i != slices) {
							glNormal3f(sinCache3a[i + 1] * sintemp3, cosCache3a[i + 1] * sintemp3, costemp3);
						}
						break;
					case GLU_NONE:
					default:
						break;
					}
					glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
				}
			}
			glEnd();
		}
		else {
			start = 0;
			finish = stacks;
		}
		for (j = start; j < finish; j++) {
			zLow = cosCache1b[j];
			zHigh = cosCache1b[j + 1];
			sintemp1 = sinCache1b[j];
			sintemp2 = sinCache1b[j + 1];
			switch (normals) {
			case GLU_FLAT:
				sintemp4 = sinCache3b[j + 1];
				costemp4 = cosCache3b[j + 1];
				break;
			case GLU_SMOOTH:
				if (orientation == GLU_OUTSIDE) {
					sintemp3 = sinCache2b[j + 1];
					costemp3 = cosCache2b[j + 1];
					sintemp4 = sinCache2b[j];
					costemp4 = cosCache2b[j];
				}
				else {
					sintemp3 = sinCache2b[j];
					costemp3 = cosCache2b[j];
					sintemp4 = sinCache2b[j + 1];
					costemp4 = cosCache2b[j + 1];
				}
				break;
			default:
				break;
			}

			glBegin(GL_QUAD_STRIP);
			for (i = 0; i <= slices; i++) {
				switch (normals) {
				case GLU_SMOOTH:
					glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
					break;
				case GLU_FLAT:
				case GLU_NONE:
				default:
					break;
				}
				if (orientation == GLU_OUTSIDE) {
					if (textureCoords) {
						glTexCoord2f(1 - (float)i / slices, 1 - (float)(j + 1) / stacks);
					}
					glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
				}
				else {
					if (textureCoords) {
						glTexCoord2f(1 - (float)i / slices, 1 - (float)j / stacks);
					}
					glVertex3f(sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], zLow);
				}
				switch (normals) {
				case GLU_SMOOTH:
					glNormal3f(sinCache2a[i] * sintemp4, cosCache2a[i] * sintemp4, costemp4);
					break;
				case GLU_FLAT:
					glNormal3f(sinCache3a[i] * sintemp4, cosCache3a[i] * sintemp4, costemp4);
					break;
				case GLU_NONE:
				default:
					break;
				}
				if (orientation == GLU_OUTSIDE) {
					if (textureCoords) {
						glTexCoord2f(1 - (float)i / slices, 1 - (float)j / stacks);
					}
					glVertex3f(sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], zLow);
				}
				else {
					if (textureCoords) {
						glTexCoord2f(1 - (float)i / slices, 1 - (float)(j + 1) / stacks);
					}
					glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
				}
			}
			glEnd();
		}
		break;
	case GLU_POINT:
		glBegin(GL_POINTS);
		for (j = 0; j <= stacks; j++) {
			sintemp1 = sinCache1b[j];
			costemp1 = cosCache1b[j];
			switch (normals) {
			case GLU_FLAT:
			case GLU_SMOOTH:
				sintemp2 = sinCache2b[j];
				costemp2 = cosCache2b[j];
				break;
			default:
				break;
			}
			for (i = 0; i < slices; i++) {
				switch (normals) {
				case GLU_FLAT:
				case GLU_SMOOTH:
					glNormal3f(sinCache2a[i] * sintemp2, cosCache2a[i] * sintemp2, costemp2);
					break;
				case GLU_NONE:
				default:
					break;
				}

				zLow = j * radius / stacks;

				if (textureCoords) {
					glTexCoord2f(1 - (float)i / slices, 1 - (float)j / stacks);
				}
				glVertex3f(sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], costemp1);
			}
		}
		glEnd();
		break;
	case GLU_LINE:
	case GLU_SILHOUETTE:
		for (j = 1; j < stacks; j++) {
			sintemp1 = sinCache1b[j];
			costemp1 = cosCache1b[j];
			switch (normals) {
			case GLU_FLAT:
			case GLU_SMOOTH:
				sintemp2 = sinCache2b[j];
				costemp2 = cosCache2b[j];
				break;
			default:
				break;
			}

			glBegin(GL_LINE_STRIP);
			for (i = 0; i <= slices; i++) {
				switch (normals) {
				case GLU_FLAT:
					glNormal3f(sinCache3a[i] * sintemp2, cosCache3a[i] * sintemp2, costemp2);
					break;
				case GLU_SMOOTH:
					glNormal3f(sinCache2a[i] * sintemp2, cosCache2a[i] * sintemp2, costemp2);
					break;
				case GLU_NONE:
				default:
					break;
				}
				if (textureCoords) {
					glTexCoord2f(1 - (float)i / slices, 1 - (float)j / stacks);
				}
				glVertex3f(sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], costemp1);
			}
			glEnd();
		}
		for (i = 0; i < slices; i++) {
			sintemp1 = sinCache1a[i];
			costemp1 = cosCache1a[i];
			switch (normals) {
			case GLU_FLAT:
			case GLU_SMOOTH:
				sintemp2 = sinCache2a[i];
				costemp2 = cosCache2a[i];
				break;
			default:
				break;
			}

			glBegin(GL_LINE_STRIP);
			for (j = 0; j <= stacks; j++) {
				switch (normals) {
				case GLU_FLAT:
					glNormal3f(sintemp2 * sinCache3b[j], costemp2 * sinCache3b[j], cosCache3b[j]);
					break;
				case GLU_SMOOTH:
					glNormal3f(sintemp2 * sinCache2b[j], costemp2 * sinCache2b[j], cosCache2b[j]);
					break;
				case GLU_NONE:
				default:
					break;
				}

				if (textureCoords) {
					glTexCoord2f(1 - (float)i / slices, 1 - (float)j / stacks);
				}
				glVertex3f(sintemp1 * sinCache1b[j], costemp1 * sinCache1b[j], cosCache1b[j]);
			}
			glEnd();
		}
		break;
	default:
		break;
	}
}
