varying vec2 pxl_texture;

uniform sampler2D pg_texture;
uniform int pg_sep_offset;
uniform int pg_sep_type;

vec4 ComputeColour()
{
	vec4 in_color = texture2D( pg_texture, pxl_texture);
	vec4 color;
	color.r = min( 1.0, (in_color.r * 0.393) + (in_color.g * 0.769) + (in_color.b * 0.189));
	color.g = min( 1.0, (in_color.r * 0.349) + (in_color.g * 0.686) + (in_color.b * 0.168));
	color.b = min( 1.0, (in_color.r * 0.272) + (in_color.g * 0.534) + (in_color.b * 0.131));
	color.a = 1.0;
	return color;
}

void main (void)  
{
	vec4 tc = vec4( 1.0, 0.0, 0.0, 1.0);
	vec2 uv = pxl_texture;

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