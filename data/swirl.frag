varying vec2 pxl_texture;

uniform sampler2D pg_texture;
uniform int pg_width;
uniform int pg_height;

void main (void)  
{
	float radius = 200.0;
	float angle = 0.8;
	vec2 center = vec2( float( pg_width) / 2.0, float( pg_height) / 2.0);

	vec2 uv = pxl_texture;
	vec2 texSize = vec2( float( pg_width), float( pg_height));	
	vec2 tc = uv * texSize;
	tc -= center;
	float dist = length(tc);
	if (dist < radius)
	{
		float percent = (radius - dist) / radius;
		float theta = percent * percent * angle * 8.0;
		float s = sin(theta);
		float c = cos(theta);
		tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));
	}
	tc += center;
	vec4 color = texture2D(pg_texture, tc / texSize);

	gl_FragColor = vec4(color.rgb, 1.0);
}