// by @eddbiddulph
// depth of field. works best in the 1 or 0.5 modes...

#ifdef GL_ES
precision highp float;
#endif

uniform float   pg_time;
uniform vec2    pg_width;
uniform vec2    pg_height;

const vec3 eps = vec3(0.001, 0.0, 0.0);

float f(vec3 p)
{
    return length(max(vec3(0.0, 0.0, 0.0), abs(mod(p, 1.0) - 0.5) - 0.15)) - 0.1;
}

vec3 n(vec3 p)
{
    float d = f(p);
    return normalize(vec3(d - f(p - eps.xyz), d - f(p - eps.zxy), d - f(p - eps.yzx)));
}

void shade(vec3 p, vec3 e, vec3 norm, vec3 op, out vec3 diff, out vec3 spec)
{
    vec3 fp = floor(p), col = mix(vec3(0.2, 0.2, 0.2), vec3(1.0, 0.6, 0.6),
                                abs(cos(fp.x) * sin(fp.y + 0.2) * sin(fp.z)));
                                
    float fres = pow(1.0 - dot(e, norm), 2.0) * 2.0, a = max(0.0, 1.0 - distance(p, op) * 0.1);

    spec = vec3(fres) * a;
    diff = col * a;
}

vec3 sceneNonRefl(vec3 ro, vec3 rd)
{
    vec3 old_ro = ro;

    for(int i = 0; i < 60; ++i)
    {
        float dist = f(ro);
        
        if(abs(dist) < 0.001)
            break;
        
        ro += rd * dist * 1.5;
    }
    
    vec3 diff, spec;

    shade(ro, -rd, n(ro), old_ro, diff, spec);

    return diff;
}

vec3 sceneRefl(vec3 ro, vec3 rd)
{
    vec3 old_ro = ro;

    for(int i = 0; i < 60; ++i)
    {
        float dist = f(ro);
        
        if(abs(dist) < 0.001)
            break;
        
        ro += rd * dist * 1.5;
    }
    
    vec3 norm = n(ro),
         refl = sceneNonRefl(ro + norm * 0.01, reflect(norm, rd));
    
    vec3 diff, spec;

    shade(ro, -rd, norm, old_ro, diff, spec);

    return refl * spec + diff;
}

vec3 rotateX(vec3 v, float a)
{
    float ca = cos(a), sa = sin(a);
    return vec3(v.x, ca * v.y - sa * v.z, sa * v.y + ca * v.z);
}

vec3 rotateY(vec3 v, float a)
{
    float ca = cos(a), sa = sin(a);
    return vec3(ca * v.x - sa * v.z, v.y, sa * v.x + ca * v.z);
}

float rand1(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float rand2(vec2 co)
{
    return fract(cos(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

const int sum_num = 8;

void main(void)
{
	vec2 resolution = vec2( float( pg_width ), float( pg_height ) );
	float time = pg_time / 1000.0;
	vec2 mouse = vec2( 0.0, 0.0 );

    float t = time, ax = -(mouse.y - 0.5) * 3.14159, ay = -(mouse.x - 0.5) * 3.14159;

    // ray target on focus plane, in view space
    vec3 targ;
    targ.xy = (gl_FragCoord.xy / resolution.xy - 0.5) * vec2(resolution.x / resolution.y, 1.0) * 1.5;
    targ.z = 2.0;

    vec3 sum = vec3(0.0);

    for(int i = 0; i < sum_num; ++i)
    {
        float fi = float(i) * 100.0;
        // ray origin (eye point) in view space
        vec3 ro = vec3(rand1(gl_FragCoord.xy + fi), rand2(gl_FragCoord.xy + fi), 0.0) * 0.06;
        
        // ray direction in view space
        vec3 rd = normalize(targ - ro);

        // perform view transforms (from view space to world space. scene is defined in world space)
        rd = rotateY(rotateX(rd, ax), ay);
        ro = rotateY(rotateX(ro, ax), ay) + vec3(cos(t * 0.2), sin(t * 0.15) * 0.1, t * 0.3);
        
        sum += sceneRefl(ro, rd);
    }

    gl_FragColor.a = 1.0;
    gl_FragColor.rgb = sum / float(sum_num);
}