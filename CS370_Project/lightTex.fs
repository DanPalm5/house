uniform sampler2D texMap;

void main()
{
    gl_FragColor = gl_Color;

	// Sample texture
	vec4 texColor = texture2D(texMap, gl_TexCoord[0].st);

	// Apply texture map using replacement
	gl_FragColor = texColor;

	gl_FragColor = texColor * gl_Color;
}