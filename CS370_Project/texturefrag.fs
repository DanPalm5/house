/* Texture map fragment shader */

// Sampler variable
uniform sampler2D texMap;

void main()
{
	// Sample texture
	vec4 texColor = texture2D(texMap, gl_TexCoord[0].st);

	// Apply texture map using replacement
	gl_FragColor = texColor;
}