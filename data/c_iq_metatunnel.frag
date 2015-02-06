// 'Metatunnel' by TX95 (2009)
// Based on a production from the demoscene, 
// the 1k intro by FRequency (http://www.pouet.net/prod.php?which=52777). 
// It made 1st position in the Numerica Artparty

#ifdef GL_ES
precision highp float;
#endif

uniform int pg_width;
uniform int pg_height;
uniform int pg_time;

float h(vec3 q, float time)
{
    float f=1.*distance(q,vec3(cos(time)+sin(time*.2),.3,2.+cos(time*.5)*.5));
    f*=distance(q,vec3(-cos(time*.7),.3,2.+sin(time*.5)));
    f*=distance(q,vec3(-sin(time*.2)*.5,sin(time),2.));
    f*=cos(q.y)*cos(q.x)-.1-cos(q.z*7.+time*7.)*cos(q.x*3.)*cos(q.y*4.)*.1;
    return f;
}

void main(void)
{
	vec2 resolution = vec2( pg_width, pg_height );
	float time = pg_time / 1000.0;
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
    vec3 o=vec3(p.x,p.y*1.25-0.3,0.);
    vec3 d=vec3(p.x+cos(time)*0.3,p.y,1.)/64.;
    vec4 c=vec4(0.);
    float t=0.;
    for(int i=0;i<75;i++)
    {
        if(h(o+d*t,time)<.4)
        {
            t-=5.;
            for(int j=0;j<5;j++)
            {
                if(h(o+d*t,time)<.4)
                    break;
                t+=1.;
            }
            vec3 e=vec3(.01,.0,.0);
            vec3 n=vec3(.0);
            n.x=h(o+d*t,time)-h(vec3(o+d*t+e.xyy),time);
            n.y=h(o+d*t,time)-h(vec3(o+d*t+e.yxy),time);
            n.z=h(o+d*t,time)-h(vec3(o+d*t+e.yyx),time);
            n=normalize(n);
            c+=max(dot(vec3(.0,.0,-.5),n),.0)+max(dot(vec3(.0,-.5,.5),n),.0)*.5;
            break;
        }
        t+=5.;
    }
    gl_FragColor=c+vec4(.1,.2,.5,1.)*(t*.025);
}
