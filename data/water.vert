attribute vec2 vertex;
attribute vec2 texture;
varying vec2 pxl_texture;
varying vec2 pxl_texture1;

varying vec3 ecPos, nor, lightDir;
uniform int pg_time;

void main(void)
{
	float Time = float( pg_time) / 1000.0;
	float WaveLen = 1.0;
	ecPos = vec3( vertex, 0.0 );
	lightDir = gl_LightSource[0].position.xyz - ecPos;
	nor = vec3(gl_NormalMatrix * gl_Normal);
	pxl_texture = texture;
	gl_Position = vec4( vertex, 0.0, 1.0 );

	mat4 RemappingMatrix = mat4( 0.5, 0.0, 0.0, 0.0, 
                                0.0, 0.5, 0.0, 0.0, 
                                0.0, 0.0, 0.5, 0.0, 
                                0.5, 0.5, 0.5, 1.0 );

	pxl_texture = RemappingMatrix * vec4( vertex, 0.0, 1.0 );
	pxl_texture1 = gl_TextureMatrix[0] * vec4( pxl_texture, 0.0, 1.0 );
	pxl_texture1.x = gl_TexCoord[1].x / WaveLen + Time / 15000.0;
	pxl_texture1.y = gl_TexCoord[1].y / WaveLen + Time / 20000.0;
}
