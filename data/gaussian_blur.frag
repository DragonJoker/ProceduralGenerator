varying vec2 pxl_texture;

uniform sampler2D pg_texture;
uniform int pg_width;
uniform int pg_height;

void main (void)  
{
	float l_fOffsetY = 1.0 / float( pg_height);
	float l_fOffsetX = 1.0 / float( pg_width);
	float l_iGaussSum = 64.0;
	vec4 l_pixelX;
	vec4 l_pixelY;

	l_pixelX  = texture2D( pg_texture, vec2( pxl_texture.x - 6.0 * l_fOffsetX, pxl_texture.y));
	l_pixelX += texture2D( pg_texture, vec2( pxl_texture.x - 5.0 * l_fOffsetX, pxl_texture.y)) * 6.0;
	l_pixelX += texture2D( pg_texture, vec2( pxl_texture.x - 4.0 * l_fOffsetX, pxl_texture.y)) * 15.0;
	l_pixelX += texture2D( pg_texture, vec2( pxl_texture.x - 3.0 * l_fOffsetX, pxl_texture.y)) * 20.0;
	l_pixelX += texture2D( pg_texture, vec2( pxl_texture.x - 2.0 * l_fOffsetX, pxl_texture.y)) * 15.0;
	l_pixelX += texture2D( pg_texture, vec2( pxl_texture.x - 1.0 * l_fOffsetX, pxl_texture.y)) * 6.0;
	l_pixelX += texture2D( pg_texture, vec2( pxl_texture.x - 0.0 * l_fOffsetX, pxl_texture.y));
	l_pixelX /= l_iGaussSum;

	l_pixelY  = texture2D( pg_texture, vec2( pxl_texture.x, pxl_texture.y - 6.0 * l_fOffsetY));
	l_pixelY += texture2D( pg_texture, vec2( pxl_texture.x, pxl_texture.y - 5.0 * l_fOffsetY)) * 6.0;
	l_pixelY += texture2D( pg_texture, vec2( pxl_texture.x, pxl_texture.y - 4.0 * l_fOffsetY)) * 15.0;
	l_pixelY += texture2D( pg_texture, vec2( pxl_texture.x, pxl_texture.y - 3.0 * l_fOffsetY)) * 20.0;
	l_pixelY += texture2D( pg_texture, vec2( pxl_texture.x, pxl_texture.y - 2.0 * l_fOffsetY)) * 15.0;
	l_pixelY += texture2D( pg_texture, vec2( pxl_texture.x, pxl_texture.y - 1.0 * l_fOffsetY)) * 6.0;
	l_pixelY += texture2D( pg_texture, vec2( pxl_texture.x, pxl_texture.y - 0.0 * l_fOffsetY));
	l_pixelY /= l_iGaussSum;

	vec4 color = (l_pixelX + l_pixelY) / 2.0;
	gl_FragColor = vec4( color.rgb, 1.0);
}