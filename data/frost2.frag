uniform sampler2D pg_texture;
uniform int pg_sep_offset;
uniform int pg_sep_type;
uniform float pg_width;
uniform float pg_height;

float semi_rand(vec2 co)
{
	float rnd_scale = 5.1;
	vec2 v1 = vec2( 92.0, 80.0);
	vec2 v2 = vec2( 41.0, 62.0);
  return fract(sin(dot(co.xy ,v1)) + cos(dot(co.xy ,v2)) * rnd_scale);
}

vec4 ComputeColour()
{
	vec2 uv = gl_TexCoord[0].xy;
	float rnd_factor = 0.05;

    vec2 rnd = vec2(semi_rand( uv.xy), semi_rand( uv.yx));
    return vec4( texture2D( pg_texture, uv + rnd * rnd_factor).rgb, 1.0);
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
		else if (uv.x > (float( pg_sep_offset) / 100.0) + 0.005)
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
		else if (uv.y > (float( pg_sep_offset) / 100.0) + 0.005)
		{
			tc = texture2D( pg_texture, uv);
		}
	}

	gl_FragColor = tc;
}
