uniform sampler2D pg_texture;
uniform int pg_sep_offset;
uniform int pg_sep_type;

vec4 ComputeColour()
{
	float hatch_y_offset = 5.0;
	float lum_threshold_1 = 1.0;
	float lum_threshold_2 = 0.7;
	float lum_threshold_3 = 0.5;
	float lum_threshold_4 = 0.3;
	vec2 uv = gl_TexCoord[0].xy;

	vec3 tc = vec3(1.0, 0.0, 0.0);
	float lum = length(texture2D(pg_texture, uv).rgb);
	tc = vec3(1.0, 1.0, 1.0);

	if (lum < lum_threshold_1)
	{
		if (mod(gl_FragCoord.x + gl_FragCoord.y, 10.0) == 0.0)
			tc = vec3(0.0, 0.0, 0.0);
	}  

	if (lum < lum_threshold_2)
	{
		if (mod(gl_FragCoord.x - gl_FragCoord.y, 10.0) == 0.0)
			tc = vec3(0.0, 0.0, 0.0);
	}  

	if (lum < lum_threshold_3)
	{
		if (mod(gl_FragCoord.x + gl_FragCoord.y - hatch_y_offset, 10.0) == 0.0)
			tc = vec3(0.0, 0.0, 0.0);
	}  

	if (lum < lum_threshold_4)
	{
		if (mod(gl_FragCoord.x - gl_FragCoord.y - hatch_y_offset, 10.0) == 0.0)
			tc = vec3(0.0, 0.0, 0.0);
	}

	return vec4(tc, 1.0);
}

void main()
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