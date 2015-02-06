uniform sampler2D pg_texture;
uniform int pg_width;
uniform int pg_height;
uniform int pg_sep_offset;
uniform int pg_sep_type;

vec4 ComputeColour()
{
	float stitching_size = 6.0;
	int invert = 1;

	vec4 c = vec4(0.0);
	float size = stitching_size;
	vec2 cPos = gl_TexCoord[0].xy * vec2(float( pg_width), float( pg_height));
	vec2 tlPos = floor(cPos / vec2(size, size));
	tlPos *= size;
	int remX = int(mod(cPos.x, size));
	int remY = int(mod(cPos.y, size));
	
	if (remX == 0 && remY == 0)
	{
		tlPos = cPos;
	}
	
	vec2 blPos = tlPos;
	blPos.y += (size - 1.0);
	
	if ((remX == remY) || (((int(cPos.x) - int(blPos.x)) == (int(blPos.y) - int(cPos.y)))))
	{
		if (invert == 1)
		{
			c = vec4(0.2, 0.15, 0.05, 1.0);
		}
		else
		{
			c = texture2D( pg_texture, tlPos * vec2(1.0/float( pg_width), 1.0/float( pg_height))) * 1.4;
		}
	}
	else
	{
		if (invert == 1)
		{
			c = texture2D( pg_texture, tlPos * vec2(1.0/float( pg_width), 1.0/float( pg_height))) * 1.4;
		}
		else
		{
			c = vec4(0.0, 0.0, 0.0, 1.0);
		}
	}
	return c;
}

void main (void)
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