uniform sampler2D pg_texture;

void main (void)  
{
	vec4 color = texture2D( pg_texture, gl_TexCoord[0]);
	gl_FragColor = color;
}