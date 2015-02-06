varying vec4 posPos;
uniform float FXAA_SUBPIX_SHIFT = 1.0/4.0;
uniform int pg_width;
uniform int pg_height;

void main(void)
{
  gl_Position = ftransform();
  gl_TexCoord[0] = gl_MultiTexCoord0;
  vec2 rcpFrame = vec2(1.0/float( pg_width), 1.0/float( pg_height));
  posPos.xy = gl_MultiTexCoord0.xy;
  posPos.zw = gl_MultiTexCoord0.xy -
                  (rcpFrame * (0.5 + FXAA_SUBPIX_SHIFT));
}