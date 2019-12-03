// Sampler variables
uniform sampler2D colorMap;
uniform sampler2D normalMap;

// TODO: Varying light and viewer variables
varying vec3 lightv;
varying vec3 viewv;

void main()
{
	// TODO: Normalized light vector
	vec3 Light = normalize(lightv);
  
	// Sample texture and normal map
	vec4 texColor = texture2D(colorMap, gl_TexCoord[0].st);
	vec4 bump = texture2D(normalMap, gl_TexCoord[0].st);

	// TODO: Unpack normal vector
	vec3 normal = normalize(2.0*bump.rgb - 1.0);

	// TODO: Compute diffuse component
	float diffuse = max(dot(Light,normal),0.0);

	// TODO: Blend lighting with texture
	gl_FragColor = diffuse*texColor;

}
