varying vec2 pxl_texture;

uniform sampler2D pg_texture;
uniform int pg_sep_type;
uniform int pg_sep_offset;

void main ()
{
	vec2 uv = pxl_texture.xy;
	vec4 c = vec4( 1.0, 0.0, 0.0, 1.0);

	if (pg_sep_type == 0)
	{
		c = texture2D( pg_texture, uv);

		c += texture2D(pg_texture, uv+0.001);
		c += texture2D(pg_texture, uv+0.003);
		c += texture2D(pg_texture, uv+0.005);
		c += texture2D(pg_texture, uv+0.007);
		c += texture2D(pg_texture, uv+0.009);
		c += texture2D(pg_texture, uv+0.011);

		c += texture2D(pg_texture, uv-0.001);
		c += texture2D(pg_texture, uv-0.003);
		c += texture2D(pg_texture, uv-0.005);
		c += texture2D(pg_texture, uv-0.007);
		c += texture2D(pg_texture, uv-0.009);
		c += texture2D(pg_texture, uv-0.011);

		c.rgb = vec3((c.r+c.g+c.b)/3.0);
		c = c / 9.5;
	}
	else if (pg_sep_type == 1)
	{
		if (uv.x < (float( pg_sep_offset) / 100.0) - 0.005)
		{
			c = texture2D( pg_texture, uv);

			c += texture2D(pg_texture, uv+0.001);
			c += texture2D(pg_texture, uv+0.003);
			c += texture2D(pg_texture, uv+0.005);
			c += texture2D(pg_texture, uv+0.007);
			c += texture2D(pg_texture, uv+0.009);
			c += texture2D(pg_texture, uv+0.011);

			c += texture2D(pg_texture, uv-0.001);
			c += texture2D(pg_texture, uv-0.003);
			c += texture2D(pg_texture, uv-0.005);
			c += texture2D(pg_texture, uv-0.007);
			c += texture2D(pg_texture, uv-0.009);
			c += texture2D(pg_texture, uv-0.011);

			c.rgb = vec3((c.r+c.g+c.b)/3.0);
			c = c / 9.5;
		}
		else if (uv.x >= (float( pg_sep_offset) / 100.0) + 0.005)
		{
			c = texture2D( pg_texture, uv);
		}
	}
	else if (pg_sep_type == 2)
	{
		if (uv.y < (float( pg_sep_offset) / 100.0) - 0.005)
		{
			c = texture2D( pg_texture, uv);

			c += texture2D(pg_texture, uv+0.001);
			c += texture2D(pg_texture, uv+0.003);
			c += texture2D(pg_texture, uv+0.005);
			c += texture2D(pg_texture, uv+0.007);
			c += texture2D(pg_texture, uv+0.009);
			c += texture2D(pg_texture, uv+0.011);

			c += texture2D(pg_texture, uv-0.001);
			c += texture2D(pg_texture, uv-0.003);
			c += texture2D(pg_texture, uv-0.005);
			c += texture2D(pg_texture, uv-0.007);
			c += texture2D(pg_texture, uv-0.009);
			c += texture2D(pg_texture, uv-0.011);

			c.rgb = vec3((c.r+c.g+c.b)/3.0);
			c = c / 9.5;
		}
		else if (uv.y >= (float( pg_sep_offset) / 100.0) + 0.005)
		{
			c = texture2D( pg_texture, uv);
		}
	}
	
	gl_FragColor = c;
}