uniform sampler2D pg_texture;
uniform int pg_sep_offset;
uniform int pg_sep_type;

vec4 ComputeColour()
{
	float gamma = 0.6;
	float numColors = 8.0;
	vec3 c = texture2D( pg_texture, gl_TexCoord[0].xy).rgb;
	c = pow(c, vec3(gamma, gamma, gamma));
	c = c * numColors;
	c = floor(c);
	c = c / numColors;
	c = pow(c, vec3(1.0/gamma));
	return vec4(c, 1.0);
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