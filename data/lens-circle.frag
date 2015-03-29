varying vec2 pxl_texture;

uniform sampler2D pg_texture;

void main()
{
	vec2 lensRadius = vec2( 0.45, 0.38);
	vec4 Color = texture2D(pg_texture, pxl_texture.xy);
	float dist = distance(pxl_texture.xy, vec2(0.5,0.5));
	Color.rgb *= smoothstep(lensRadius.x, lensRadius.y, dist);
	gl_FragColor = Color;
}