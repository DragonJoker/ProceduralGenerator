uniform sampler2D Tex0, Tex1, Tex2;
uniform float WaveHeight, R0;
varying vec3 ecPos, nor, lightDir;

void main(void)
{
	float fresnel, specular;
	vec3 normal, lightVec, refVec;

	vec4 normalMap = texture2D(Tex2, gl_TexCoord[1].xy);
	normal = normalize(2.0 * normalMap.rgb - 1.0); // decoding normal map

	vec3 ProjCoords = gl_TexCoord[0].xyz;
	ProjCoords.xy += WaveHeight* normal.rg; // perturbation

	vec4 refractionMap = texture2DProj(Tex0, ProjCoords);
	vec4 reflectionMap = texture2DProj(Tex1, ProjCoords);

	lightVec = normalize(lightDir);

//	fresnel = 1.0 - (R0 + (1.0 - R0) * pow(1.0 - dot(normalize(-ecPos), normalize(nor)), 5.0));
	fresnel = dot(normalize(-ecPos), normalize(nor));
//	fresnel = 0.0;

	refVec = reflect(-lightVec, normal);
	specular = max(dot(normalize(refVec), normalize(-ecPos)), 0.0);
	specular = pow(specular, 256.0);
//	specular = 0.0;
	
	gl_FragColor = mix(reflectionMap, refractionMap, fresnel) + specular;
//	gl_FragColor = vec4(fresnel);
//	gl_FragColor = vec4(nor, 1.0);
}
