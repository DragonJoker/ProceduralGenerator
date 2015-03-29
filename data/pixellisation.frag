varying vec2 pxl_texture;

uniform sampler2D pg_texture;
uniform int pg_width;
uniform int pg_height;
uniform int pg_sep_offset;
uniform int pg_sep_type;

vec4 ComputeColour()
{
	float pixel_w = 15.0;
	float pixel_h = 10.0;
	vec2 uv = pxl_texture;
	float dx = pixel_w*(1.0 / float( pg_width));
	float dy = pixel_h*(1.0 / float( pg_height));
	vec2 coord = vec2(dx*floor(uv.x/dx), dy*floor(uv.y/dy));
	return texture2D( pg_texture, coord);
}

void main()
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