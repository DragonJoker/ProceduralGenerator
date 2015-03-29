varying vec2 pxl_texture;

uniform sampler2D pg_texture;
uniform int pg_sep_offset;
uniform int pg_sep_type;

vec4 ComputeColour()
{
	vec4 in_color = texture2D( pg_texture, pxl_texture);
	float luminance =  sqrt( 0.241f * in_color.r * in_color.r + 0.691f * in_color.g * in_color.g + 0.068f * in_color.b * in_color.b );
	vec4 color;
	color.r = min( 1.0, luminance);
	color.g = min( 1.0, luminance);
	color.b = min( 1.0, luminance);
	color.a = 1.0;
	return color;
}

void main (void)  
{
	vec4 tc = vec4( 1.0, 0.0, 0.0, 1.0);
	vec2 uv = pxl_texture.xy;

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