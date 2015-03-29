varying vec2 pxl_texture;

uniform sampler2D pg_texture;

void main (void)  
{
	vec2 l_ptCen = vec2( 0.5, 0.5) - pxl_texture.xy;
	vec2 l_ptMCen = -0.07 * log( length( l_ptCen)) * normalize( l_ptCen);
	vec4 color = texture2D( pg_texture, pxl_texture.xy + l_ptMCen);
	gl_FragColor = color;
}