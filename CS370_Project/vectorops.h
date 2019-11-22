// Routine to normalize a vector
void normalize(GLfloat* a)
{
	float norm;
	int i;

	norm = 0.0f;
	for (i = 0; i < 3; i++)
	{
		norm += a[i] * a[i];
	}
	norm = sqrt(norm);

	if (norm > 0.0)
	{
		for (i = 0; i < 3; i++)
		{
			a[i] = a[i] / norm;
		}
	}
}

void cross(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* n)
{
	n[0] = (b[1] - a[1]) * (c[2] - a[2]) - (b[2] - a[2]) * (c[1] - a[1]);
	n[1] = (b[2] - a[2]) * (c[0] - a[0]) - (b[0] - a[0]) * (c[2] - a[2]);
	n[2] = (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]);
	normalize(n);
}
