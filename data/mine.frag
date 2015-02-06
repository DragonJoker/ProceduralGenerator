uniform int pg_width;
uniform int pg_height;
uniform int pg_time;

float seed;
float pi;

float fnorm( float f, float off )
{
	return off * f + off;
}
float rand( vec2 co )
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

// Pseudo random number base generator (credits go to iq/rgba)
float rnd(vec2 x)
{
	int n = int(x.x * 40.0 + x.y * 6400.0);
	n = (n << 13) ^ n;
	return 1.0 - float( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

float noise( vec2 x, float f )
{
	return rand(x) * 0.1 + f;
}

vec4 sky( float time, float x, float dist )
{
	vec4 base = vec4( max( 0.0, 0.5 - dist ), max( 0.0, 0.5 - dist ), 1.0, 1.0 );
	return base;
}

vec4 sea( float time, float x, float dist )
{
	vec4 base = vec4( max( 0.0, 0.5 - dist ), max( 0.0, 0.5 - dist ), 1.0, 1.0 );
	return base;
}

vec4 ground( float time, vec2 relative, float mx )
{
	vec2 tmp = vec2( relative.x, relative.y / mx );
	return vec4( fnorm( rand( tmp ), 0.7 ), fnorm( rand( tmp ), 0.7 ), 0.0, 1.0 );
}

void main()
{
	seed = 11.0;
	pi = 3.141592;
	vec2 relative = gl_FragCoord.xy / vec2( pg_width, pg_height );
	float time = pg_time / 1000.0;
	float limitSea = fnorm( sin( time ) + (cos( relative.x * 20.0 + time * 2.0 ) * 0.5) + (sin( relative.x * 17.0 + time * 2.0 ) * 0.4), 0.05 ) + 0.45;
	float limitGround = fnorm( (cos( relative.x * 20.0 ) * 0.5) + (sin( relative.x * 17.0 ) * 0.4), 0.05 ) + 0.1 + rand( relative )*0.01;
	float f = fnorm( cos( time ), 0.5 );
	
	if( relative.y > limitSea )
	{
		gl_FragColor = sky( time, relative.x, relative.y - 0.5 );
	}
	else if( relative.y > limitGround )
	{
		gl_FragColor = sea( time, relative.x, 0.5 - relative.y );
	}
	else
	{
		gl_FragColor = ground( time, relative, 0.15 );
	}
}