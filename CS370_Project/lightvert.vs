/* Lighting vertex shader */
uniform int numLights;

// Shader routine for directional light sources
void DirectionalLight(in int i, 
					  in vec3 normal, 
					  inout vec4 ambient, 
					  inout vec4 diffuse, 
					  inout vec4 specular)
{
	float nDotVP;	// normal . light direction
	float nDotHV;	// normal . light half vector
	float pf;		// power factor

	// Compute diffuse angle
	nDotVP = max(0.0, dot(normal,normalize(vec3(gl_LightSource[i].position))));

	// Compute specular angle
	nDotHV = max(0.0, dot(normal,vec3(gl_LightSource[i].halfVector)));

	// Apply shininess factor
	if (nDotVP == 0.0)
	{
		pf = 0.0;
	}
	else
	{
		pf = pow(nDotHV, gl_FrontMaterial.shininess);
	}

	// Accumulate components
	ambient += gl_LightSource[i].ambient;
	diffuse += gl_LightSource[i].diffuse * nDotVP;
	specular += gl_LightSource[i].specular * pf;
}

// Shader routine for point light sources
void PointLight(in int i,
				in vec3 eye,
				in vec3 ecPosition3,
				in vec3 normal, 
				inout vec4 ambient, 
				inout vec4 diffuse, 
				inout vec4 specular)
{
	float nDotVP;		// normal . light direction
	float nDotHV;		// normal . light half vector
	float pf;			// power factor
	float attenuation; 	// computed attenuation factor
	float d;			// distance from surface to light source
	vec3 VP;			// direction from surface to light position
	vec3 halfVector;	// direction of maximum highlights
	
	// Compute vector from surface to light position
	VP = vec3(gl_LightSource[i].position) - ecPosition3;
	
	// Compute distance between surface and light position
	d = length(VP);
	
	// Normalize the vector from surface to light position
	VP = normalize(VP);
	
	// Compute attenuation
	attenuation = 1.0/ (gl_LightSource[i].constantAttenuation + 
						gl_LightSource[i].linearAttenuation * d + 
						gl_LightSource[i].quadraticAttenuation * d * d);
						
	halfVector = normalize(VP + eye);
	nDotVP = max(0.0,dot(normal,VP));
	nDotHV = max(0.0,dot(normal,halfVector));
	
	if (nDotVP == 0.0)
		pf = 0.0;
	else
		pf = pow(nDotHV, gl_FrontMaterial.shininess);

	// Accumulate components
	ambient += gl_LightSource[i].ambient * attenuation;
	diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;
	specular += gl_LightSource[i].specular * pf * attenuation;
}

// Shader routine for spot light sources
void SpotLight(in int i,
				in vec3 eye,
				in vec3 ecPosition3,
				in vec3 normal, 
				inout vec4 ambient, 
				inout vec4 diffuse, 
				inout vec4 specular)
{
	float nDotVP;			// normal . light direction
	float nDotHV;			// normal . light half vector
	float pf;				// power factor
	float spotDot;			// cosine of angle between spotlight
	float spotAttenuation; 	// spotlight attenuation factor
	float attenuation;		// computed attenuation factor
	float d;				// distance from surface to light source
	vec3 VP;				// direction from surface to light position
	vec3 halfVector;		// direction of maximum highlights
	
	// Compute vector from surface to light position
	VP = vec3(gl_LightSource[i].position) - ecPosition3;
	
	// Compute distance between surface and light position
	d = length(VP);
	
	// Normalize the vector from surface to light position
	VP = normalize(VP);
	
	// Compute attenuation
	attenuation = 1.0/ (gl_LightSource[i].constantAttenuation + 
						gl_LightSource[i].linearAttenuation * d + 
						gl_LightSource[i].quadraticAttenuation * d * d);

	// Check if point on surface is inside cone of illumination
	spotDot = dot(-VP,normalize(gl_LightSource[i].spotDirection));
	
	if (spotDot < gl_LightSource[i].spotCosCutoff)
		spotAttenuation = 0.0;	// light adds no contribution
	else
		spotAttenuation = pow(spotDot,gl_LightSource[i].spotExponent);
		
	// Combine the spotlight and distance attenuation
	attenuation *= spotAttenuation;
						
	halfVector = normalize(VP + eye);
	nDotVP = max(0.0,dot(normal,VP));
	nDotHV = max(0.0,dot(normal,halfVector));
	
	if (nDotVP == 0.0)
		pf = 0.0;
	else
		pf = pow(nDotHV, gl_FrontMaterial.shininess);

	// Accumulate components
	ambient += gl_LightSource[i].ambient * attenuation;
	diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;
	specular += gl_LightSource[i].specular * pf * attenuation;
}

void main()
{
	// Lighting accumulators
	// Set ambient light
	vec4 amb = gl_LightModel.ambient;
	vec4 diff = vec4(0.0);
	vec4 spec = vec4(0.0);
	vec3 normal = vec3(0.0);
	vec4 ecPosition;
	vec3 ecPosition3;
	vec3 eye = vec3(0.0,0.0,1.0);
	vec4 color = vec4(0.0,1.0,0.0,1.0);
	
	// Set ambient alpha for blending
	amb.a = 0.0;

	// Compute transformed vertex position
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;

	// Transform vertex to eye coordinates
	ecPosition = gl_ModelViewMatrix * gl_Vertex;
	ecPosition3 = (vec3(ecPosition)) / ecPosition.w;

	// Compute transformed (normalized) normal
	normal = normalize(gl_NormalMatrix * gl_Normal);

	// Loop through light sources
	for (int i = 0; i < numLights; i++)
	{
		// Directional lights
		if (gl_LightSource[i].position.w == 0.0)
		{
			DirectionalLight(i, normal, amb, diff, spec);
		}
		// Point lights
		else if (gl_LightSource[i].spotCutoff == 180.0)
		{
			PointLight(i, eye, ecPosition3, normal, amb, diff, spec);
		}
		// Spot lights
		else
		{
			SpotLight(i, eye, ecPosition3, normal, amb, diff, spec);
		}

	}

	// Compute final lighting color
	gl_FrontColor = amb*gl_FrontMaterial.ambient + diff*gl_FrontMaterial.diffuse + spec*gl_FrontMaterial.specular;

	// gl_TexCoord[0] = gl_MultiTexCoord0;
}
