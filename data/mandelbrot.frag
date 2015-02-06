uniform sampler2D pg_texture;

uniform int pg_width;
uniform int pg_height;

vec2 mulComplex(vec2 a, vec2 b)
{
	float x = -a.y*b.y+a.x*b.x;
	float y = a.x*b.y+b.x*a.y;
	
	return vec2(x, y);
}

void main()
{
	float ratio_image = float( pg_width ) / float( pg_height );
	float nbIterations = 100.0;
	vec2 pointCentral = vec2( -0.5, 0.0 );
	float zoom = 0.35;
/*
	float nbIterations = 400.0;
	vec2 pointCentral = vec2( -1.028716, 0.075906 );
	float zoom = 782.85;
*/
	vec4 pixel = vec4( 0.0, 0.0, 0.0, 0.0 );
	
	// on calcule la fractale de mandelbrot
	vec2 z = (gl_TexCoord[0].xy - vec2(0.5, 0.5)) / zoom + pointCentral;
	z.x *= ratio_image;
	vec2 z0 = z;	
	float i = 0.0;
	
	while( i < nbIterations )
	{
		z = mulComplex(z, z) + z0;
		
		if(z.x * z.x + z.y * z.y > 4.0)
		{
			break;
		}
		
		i += 1.0;
	}
	
	if(i == nbIterations)
	{
		i = 0.0;
	}
	
	i /= nbIterations;
	
	// noir et blanc
//	pixel = vec4(i);

	// en utilisant le dégradé
	if(i == 0)
		pixel = vec4(0);
	else
		pixel = texture2D(pg_texture, vec2(i, 0));

	gl_FragColor = /*vec4( z.x, z.y, 0.0, 1.0 );*/pixel;
}
