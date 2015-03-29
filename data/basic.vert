attribute vec2 vertex;
attribute vec2 texture;
varying vec2 pxl_texture;

void main()
{
	pxl_texture = texture;
	gl_Position = vec4( vertex, 0.0, 1.0 );
}