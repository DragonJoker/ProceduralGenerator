uniform sampler2D pg_texture;
uniform int pg_sep_offset;
uniform int pg_sep_type;

vec4 ComputeColour()
{
	vec3 pixcol = texture2D(pg_texture, gl_TexCoord[0]).rgb;
	vec3 colors[3];
	colors[0] = vec3(0.,0.,1.);
	colors[1] = vec3(1.,1.,0.);
	colors[2] = vec3(1.,0.,0.);
	float lum = (pixcol.r+pixcol.g+pixcol.b)/3.;
	int ix = (lum < 0.5)? 0:1;
	return vec4( mix(colors[ix],colors[ix+1],(lum-float(ix)*0.5)/0.5), 1.0);
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