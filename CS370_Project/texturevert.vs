/* Texture vertex shader */

void main()
{
	// Compute transformed vertex position
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
	
	// TODO: Pass texture coordinate from texture unit 0
	gl_TexCoord[0] = gl_MultiTexCoord0;

}
