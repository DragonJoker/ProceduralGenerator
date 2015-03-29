attribute vec2 vertex;

uniform int pg_time;

varying vec3 s[4];

void main()
{
	gl_Position = vec4( vertex, 0.0, 1.0 );
	s[0] = vec3( 0 );
	s[3] = vec3( sin( abs( pg_time * 0.001 ) ), cos( abs( pg_time * .001 ) ), 0 );
	s[1] = s[3].zxy;
	s[2] = s[3].zzx;
}