// 'You Massive Clod' by Tigrou (2008)
// Code for the 1k intro You Massive Clod from the demoscene (http://www.pouet.net/prod.php?which=51762). 
// It made 5th position in the Function 2008 demoparty

#ifdef GL_ES
precision highp float;
#endif

uniform int pg_width;
uniform int pg_height;
uniform int pg_time;

float f(vec3 o)
{
    float a=(sin(o.x)+o.y*.25)*.35;
    o=vec3(cos(a)*o.x-sin(a)*o.y,sin(a)*o.x+cos(a)*o.y,o.z);
    return dot(cos(o)*cos(o),vec3(1))-1.2;
}
vec3 s(vec3 o,vec3 d)
{
    float t=0.,a,b;
    for(int i=0;i<75;i++)
    {
        if(f(o+d*t)<0.0)
        {
            a=t-.125;b=t;
            for(int i=0; i<10;i++)
            {
                t=(a+b)*.5;
                if(f(o+d*t)<0.0)
                    b=t;
                else 
                    a=t;
            }
            vec3 e=vec3(.1,0.0,0.0);
            vec3 p=o+d*t;
            vec3 n=-normalize(vec3(f(p+e),f(p+e.yxy),f(p+e.yyx))+vec3((sin(p*75.)))*.01);

            return vec3( mix( ((max(-dot(n,vec3(.577)),0.) + 0.125*max(-dot(n,vec3(-.707,-.707,0)),0.)))*(mod(length(p.xy)*20.,2.)<1.0?vec3(.71,.85,.25):vec3(.79,.93,.4))
                           ,vec3(.93,.94,.85), vec3(pow(t/9.,5.)) ) );
        }
        t+=.125;
    }
    return vec3(.93,.94,.85);
}

void main(void)
{
	vec2 resolution = vec2( pg_width, pg_height );
	float time = pg_time / 1000.0;
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
    gl_FragColor=vec4(s(vec3(sin(time*1.5)*.5,cos(time)*.5,time), normalize(vec3(p.xy,1.0))),1.0);
}

