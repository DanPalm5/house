// TODO: Tangent vector attribute variable
attribute vec3 tangento;

// TODO: Light vector varying variables
varying vec3 lightv;
varying vec3 viewv;

void main()
{
	// Compute transformed vertex
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;

	// Set texture coordinate
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	// TODO: Compute tangent space vectors
	vec3 n = normalize(gl_NormalMatrix*gl_Normal);
	vec3 t = normalize(gl_NormalMatrix*tangento);
	vec3 b = cross(n,t);

	// TODO: Compute viewer vector
	vec3 viewe = vec3(gl_ModelViewMatrix*gl_Vertex);
 
	// TODO: Transform viewer to tangent space
	viewv.x = dot(t,viewe.xyz);
	viewv.y = dot(b,viewe.xyz);
	viewv.z = dot(n,viewe.xyz);

	// TODO: Compute light vector
	vec3 lighte = normalize(vec3(gl_LightSource[0].position) - viewe);

	// TODO: Transform light to tangent space
	lightv.x = dot(t,lighte.xyz);
	lightv.y = dot(b,lighte.xyz);
	lightv.z = dot(n,lighte.xyz);

	// TODO: Normalize tangent space light and viewer
	lightv = normalize(lightv);
	viewv = normalize(viewv);
}
