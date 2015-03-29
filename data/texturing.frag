varying vec2 pxl_texture;

uniform sampler2D pg_texture;

void main (void)  
{
	gl_FragColor = texture2D( pg_texture, pxl_texture );
}