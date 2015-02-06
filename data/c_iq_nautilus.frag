// 'Nautilus' by Weyland Yutani (reworked by iq) (2010)
// A 1 kilobyte demo from the demoscene, using raymarching and screen space ambient occlusion.

#ifdef GL_ES
precision highp float;
#endif

uniform int pg_width;
uniform int pg_height;
uniform int pg_time;

float e(vec3 c, float time)
{
    c=cos(vec3(cos(c.r+time/6.0)*c.r-cos(c.g*3.0+time/5.0)*c.g, cos(time/4.0)*c.b/3.0*c.r-cos(time/7.0)*c.g, c.r+c.g+c.b+time));
    return dot(c*c,vec3(1.0))-1.0;
}

void main(void)
{
	vec2 resolution = vec2( pg_width, pg_height );
	float time = pg_time / 1000.0;
    vec2 c=-1.0+2.0*gl_FragCoord.rg/resolution.xy;
    vec3 o=vec3(c.r,c.g,0.0),g=vec3(c.r,c.g,1.0)/64.0,v=vec3(0.5);
    float m = 0.4;
//    float m = 1.0-1.5*mouse.x/resolution.x;
    for(int r=0;r<100;r++)
    {
      float h=e(o,time)-m;
      if(h<0.0)break;
      o+=h*10.0*g;
      v+=h*0.02;
    }
    // light (who needs a normal?)
    v+=e(o+0.1,time)*vec3(0.4,0.7,1.0);

    // ambient occlusion
    float a=0.0;
    for(int r=0;r<100;r++)
       a+=clamp(4.0*(e(o+0.5*vec3(cos(1.1*float(r)),cos(1.6*float(r)),cos(1.4*float(r))),time)-m),0.0,1.0);
    v*=a/100.0;
    gl_FragColor=vec4(v,1.0);
}
