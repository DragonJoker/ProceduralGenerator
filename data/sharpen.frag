uniform sampler2D pg_texture;
uniform int pg_width;
uniform int pg_height;
uniform int pg_sep_offset;
uniform int pg_sep_type;

vec4 ComputeColour()
{
	float l_fOffsetY = 1.0 / float( pg_height);
	float l_fOffsetX = 1.0 / float( pg_width);
	float l_fSharpenZ = 1.0;
	float l_fSharpenMask[9];
	l_fSharpenMask[0] = 0.0;
	l_fSharpenMask[1] = -l_fSharpenZ;
	l_fSharpenMask[2] = 0.0;
	l_fSharpenMask[3] = -l_fSharpenZ;
	l_fSharpenMask[4] = 1.0 + 4.0 * l_fSharpenZ;
	l_fSharpenMask[5] = -l_fSharpenZ;
	l_fSharpenMask[6] = 0.0;
	l_fSharpenMask[7] = -l_fSharpenZ;
	l_fSharpenMask[8] = 0.0;
	vec4 color;
	
	if (gl_TexCoord[0].x >= 0.0 + l_fOffsetX)
	{
		if (gl_TexCoord[0].y >= 0.0 + l_fOffsetY)
		{
			color  = texture2D( pg_texture, vec2( gl_TexCoord[0].x - l_fOffsetX,	gl_TexCoord[0].y - l_fOffsetY)) 	* l_fSharpenMask[0];
		}
		else
		{
			color  = texture2D( pg_texture, vec2( gl_TexCoord[0].x - l_fOffsetX,	gl_TexCoord[0].y)) 				* l_fSharpenMask[0];
		}
		color += texture2D( 	pg_texture, vec2( gl_TexCoord[0].x - l_fOffsetX,	gl_TexCoord[0].y)) 				* l_fSharpenMask[1];
		if (gl_TexCoord[0].y <= 1.0 - l_fOffsetY)
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x - l_fOffsetX,	gl_TexCoord[0].y + l_fOffsetY)) 	* l_fSharpenMask[2];
		}
		else
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x - l_fOffsetX,	gl_TexCoord[0].y)) 				* l_fSharpenMask[2];
		}
	}
	else
	{
		if (gl_TexCoord[0].y >= 0.0 + l_fOffsetY)
		{
			color  = texture2D( pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y - l_fOffsetY)) 	* l_fSharpenMask[0];
		}
		else
		{
			color  = texture2D( pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y)) 				* l_fSharpenMask[0];
		}
		color += texture2D( 	pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y)) 				* l_fSharpenMask[1];
		if (gl_TexCoord[0].y <= 1.0 - l_fOffsetY)
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y + l_fOffsetY)) 	* l_fSharpenMask[2];
		}
		else
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y)) 				* l_fSharpenMask[2];
		}
	}
	
	if (gl_TexCoord[0].y >= 0.0 + l_fOffsetY)
	{
		color += texture2D( 	pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y - l_fOffsetY)) 	* l_fSharpenMask[3];
	}
	else
	{
		color += texture2D( 	pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y)) 				* l_fSharpenMask[3];
	}
	color += texture2D( 		pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y)) 				* l_fSharpenMask[4];
	if (gl_TexCoord[0].y <= 1.0 - l_fOffsetY)
	{
		color += texture2D( 	pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y + l_fOffsetY)) 	* l_fSharpenMask[5];
	}
	else
	{
		color += texture2D( 	pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y)) 				* l_fSharpenMask[5];
	}
	
	if (gl_TexCoord[0].x <= 1.0 - l_fOffsetX)
	{
		if (gl_TexCoord[0].y >= 0.0 + l_fOffsetY)
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x + l_fOffsetX, gl_TexCoord[0].y - l_fOffsetY)) 	* l_fSharpenMask[6];
		}
		else
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x + l_fOffsetX, gl_TexCoord[0].y)) 				* l_fSharpenMask[6];
		}
		color += texture2D( 	pg_texture, vec2( gl_TexCoord[0].x + l_fOffsetX, gl_TexCoord[0].y)) 				* l_fSharpenMask[7];
		if (gl_TexCoord[0].y <= 1.0 - l_fOffsetY)
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x + l_fOffsetX, gl_TexCoord[0].y + l_fOffsetY)) 	* l_fSharpenMask[8];
		}
		else
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x + l_fOffsetX, gl_TexCoord[0].y)) 				* l_fSharpenMask[8];
		}
	}
	else
	{
		if (gl_TexCoord[0].y >= 0.0 + l_fOffsetY)
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y - l_fOffsetY)) 	* l_fSharpenMask[6];
		}
		else
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y)) 				* l_fSharpenMask[6];
		}
		color += texture2D( 	pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y)) 				* l_fSharpenMask[7];
		if (gl_TexCoord[0].y <= 1.0 - l_fOffsetY)
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y + l_fOffsetY)) 	* l_fSharpenMask[8];
		}
		else
		{
			color += texture2D( pg_texture, vec2( gl_TexCoord[0].x, 				gl_TexCoord[0].y)) 				* l_fSharpenMask[8];
		}
	}
	
	return vec4( clamp( color.r, 0.0, 1.0), clamp( color.g, 0.0, 1.0), clamp( color.b, 0.0, 1.0), 1.0);
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