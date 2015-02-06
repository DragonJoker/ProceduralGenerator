varying vec3 ecPos, nor, lightDir;
uniform int pg_time;

void main(void)
{
	float Time = float( pg_time) / 1000.0;
	float WaveLen = 1.0;
	ecPos = vec3( gl_ModelViewMatrix * gl_Vertex);
	lightDir = gl_LightSource[0].position.xyz - ecPos;
	nor = vec3(gl_NormalMatrix * gl_Normal);
	gl_Position = ftransform();

	mat4 RemappingMatrix = mat4(0.5, 0.0, 0.0, 0.0, 
                                0.0, 0.5, 0.0, 0.0, 
                                0.0, 0.0, 0.5, 0.0, 
                                0.5, 0.5, 0.5, 1.0);

	gl_TexCoord[0] = RemappingMatrix * (gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex);
	gl_TexCoord[1] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_TexCoord[1].x = gl_TexCoord[1].x/WaveLen + Time/15000.0;
	gl_TexCoord[1].y = gl_TexCoord[1].y/WaveLen + Time/20000.0;
}
