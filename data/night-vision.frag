uniform sampler2D pg_texture;
uniform sampler2D pg_modif1;
uniform sampler2D pg_modif2;
uniform int pg_time;
uniform int pg_sep_type;
uniform int pg_sep_offset;

vec4 ComputeColour()
{
	float luminanceThreshold = 0.2;
	float colorAmplification = 4.0;
	float elapsedTime = pg_time / 1000.0;
	vec2 uv;
	uv.x = 0.4*sin(elapsedTime*50.0);
	uv.y = 0.4*cos(elapsedTime*50.0);
	float m = texture2D(pg_modif2, gl_TexCoord[0].st).r;
	vec3 n = texture2D(pg_modif1, (gl_TexCoord[0].st*3.5) + uv).rgb;
	vec3 c = texture2D(pg_texture, gl_TexCoord[0].st + (n.xy*0.005)).rgb;

	float lum = dot(vec3(0.30, 0.59, 0.11), c);

	if (lum < luminanceThreshold)
	{
		c *= colorAmplification; 
	}

	vec3 visionColor = vec3(0.1, 0.95, 0.2);
	return vec4( (c + (n*0.2)) * visionColor * m, 1.0);
}

void main ()
{
	vec4 tc = vec4( 1.0, 0.0, 0.0, 1.0);
	vec2 uv = gl_TexCoord[0].xy;

	if (pg_sep_type == 0)
	{
		tc = ComputeColour();
	}
	else if (pg_sep_type == 1)
	{
		if (uv.x < (float( pg_sep_offset) / 100.0) - 0.005)
		{
			tc = ComputeColour();
		}
		else if (uv.x >= (float( pg_sep_offset) / 100.0) + 0.005)
		{
			tc = texture2D( pg_texture, uv);
		}
	}
	else if (pg_sep_type == 2)
	{
		if (uv.y < (float( pg_sep_offset) / 100.0) - 0.005)
		{
			tc = ComputeColour();
		}
		else if (uv.y >= (float( pg_sep_offset) / 100.0) + 0.005)
		{
			tc = texture2D( pg_texture, uv);
		}
	}

	gl_FragColor = tc;
}