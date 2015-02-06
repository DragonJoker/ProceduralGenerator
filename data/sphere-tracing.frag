#version 150

//*************************************************************************************************
//	DISTANCE OPERATIONS
//*************************************************************************************************

float lengthn( vec2 v, float n )
{
	return pow( (pow(v.x,n) + pow(v.y,n)), 1.0 / n );
}

float lengthn( vec3 v, float n )
{
	return pow( (pow(v.x,n) + pow(v.y,n) + pow(v.z,n)), 1.0 / n );
}

float length2( vec2 v )
{
	return length( v );
}

float length8( vec2 v )
{
	return lengthn( v, 8.0 );
}

// Union
float opU( float d1, float d2 )
{
	return min( d1, d2 );
}

// Substraction
float opS( float d1, float d2 )
{
	return max( -d1, d2 );
}

// Intersection
float opI( float d1, float d2 )
{
	return max( d1, d2 );
}

//*************************************************************************************************
//	PRIMITIVES
//*************************************************************************************************

float sdSphere( vec3 p, float s )
{
	return length( p ) - s;
}

float udBox( vec3 p, vec3 b )
{
	return length( max( abs( p ) - b, 0.0 ) );
}

float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs( p ) - b;
  return min( max( d.x, max( d.y, d.z ) ),0.0 ) + length( max( d, 0.0 ) );
}

float udRoundBox( vec3 p, vec3 b, float r )
{
  return length( max( abs( p ) - b, 0.0 ) ) - r;
}

float sdTorus( vec3 p, vec2 t )
{
  vec2 q = vec2( length( p.xz ) - t.x, p.y );
  return length( q ) - t.y;
}

float sdCylinder( vec3 p, vec3 c )
{
  return length( p.xz - c.xy ) - c.z;
}

float sdCone( vec3 p, vec2 c )
{
    // c must be normalized
    float q = length( p.xy );
    return dot( c, vec2( q, p.z ) );
}

float sdPlane( vec3 p, vec4 n )
{
  // n must be normalized
  return dot( p, n.xyz ) + n.w;
}

float sdHexPrism( vec3 p, vec2 h )
{
    vec3 q = abs( p );
    return max( q.z - h.y, max( q.x + q.y * 0.57735, q.y * 1.1547 ) - h.x );
}

float sdTriPrism( vec3 p, vec2 h )
{
    vec3 q = abs( p );
    return max( q.z - h.y, max( q.x * 0.866025 + p.y * 0.5, -p.y ) - h.x * 0.5 );
}

float sdTorus82( vec3 p, vec2 t )
{
  vec2 q = vec2( length2( p.xz ) - t.x, p.y );
  return length8( q ) - t.y;
}

float sdTorus88( vec3 p, vec2 t )
{
  vec2 q = vec2( length8( p.xz ) - t.x, p.y );
  return length8( q ) - t.y;
}

//*************************************************************************************************
//	DOMAIN OPERATIONS
//*************************************************************************************************

// Repetition
float opRep( vec3 p, vec3 c )
{
	vec3 q = mod( p, c ) - 0.5 * c;
//	return primitive( q );
	return q;
}

// Rotation/Translation
vec3 opTx( vec3 p, mat4 m )
{
	vec3 q = vec3( inverse( m ) * vec4( p, 1.0 ) );
//	return primitive(q);
	return q;
}

// Scale
float opScale( vec3 p, float s )
{
//	return primitive( p / s ) * s;
	return p;
}

//*************************************************************************************************
//	DISTANCE DEFORMATIONS
//*************************************************************************************************

// Displacement
float opDisplace( vec3 p )
{
//	float d1 = primitive( p );
//	float d2 = displacement( p );
//	return d1+d2;
	return 0.0;
}

// Blend
float opBlend( vec3 p )
{
//	float d1 = primitiveA( p );
//	float d2 = primitiveB( p );
//	float dd = smoothcurve( d1 - d2 );
//	return mix( d1, d2, dd );
	return 0.0;
}

//*************************************************************************************************
//	DOMAIN DEFORMATIONS
//*************************************************************************************************

// Twist
float opTwist( vec3 p )
{
    float c = cos( 20.0 * p.y );
    float s = sin( 20.0 * p.y );
    mat2  m = mat2( c, -s, s, c );
    vec3  q = vec3( m * p.xz, p.y );
//    return primitive( q );
	return 0.0;
}

// Cheap Bend
float opCheapBend( vec3 p )
{
    float c = cos( 20.0 * p.y );
    float s = sin( 20.0 * p.y );
    mat2  m = mat2( c, -s, s, c );
    vec3  q = vec3( m * p.xy, p.z);
//    return primitive(q);
	return 0.0;
}

//*************************************************************************************************
//	SPHERE TRACING
//*************************************************************************************************

int g_iMaxStep = 50;
float g_iEpsilon = 0.001;

struct Sphere
{
	float fRayon;
	vec4 v4Diffuse;
	vec4 v4Specular;
	float fShininess;
};

struct SphereIntersection
{
	Sphere sphere;
	float fDistance;
};


Sphere g_sphere1;
Sphere g_sphere2;


SphereIntersection minSI( SphereIntersection p_si1, SphereIntersection p_si2 )
{
	if( min( p_si1.fDistance, p_si2.fDistance ) == p_si1.fDistance )
	{
		return p_si1;
	}
	
	return p_si2;
}

SphereIntersection iso( vec3 p_v3Position )
{
	SphereIntersection l_sphere1;
	SphereIntersection l_sphere2;

	l_sphere1.sphere = g_sphere1;
	l_sphere2.sphere = g_sphere1;
	l_sphere1.fDistance = sdSphere( p_v3Position, g_sphere1.fRayon );	
	l_sphere2.fDistance = sdSphere( p_v3Position, g_sphere2.fRayon );
	
	return minSI( l_sphere1, l_sphere2 );
}

vec3 normal( vec3 p_v3Position, float p_epsilon )
{
	return vec3( 	length( p_v3Position + vec3( p_epsilon, 0.0, 0.0 ) ) - length( p_v3Position - vec3( p_epsilon, 0.0, 0.0 ) )
				, 	length( p_v3Position + vec3( 0.0, p_epsilon, 0.0 ) ) - length( p_v3Position - vec3( 0.0, p_epsilon, 0.0 ) )
				, 	length( p_v3Position + vec3( 0.0, 0.0, p_epsilon ) ) - length( p_v3Position - vec3( 0.0, 0.0, p_epsilon ) )
				);
}

/**
 *\brief	Algorithme de base du Sphere Tracing
 *\param	p_v3Position	Position initiale de la caméra
 *\param	p_v3Direction	Direction de la caméra
 */
void ray( vec3 p_v3Position, vec3 p_v3Direction )
{
	int l_iStep = 0;
	SphereIntersection l_siDistance;			// distance vers l'objet  
	vec3 l_v3Position = p_v3Position;	// prochaine position de la caméra

	do
	{
		l_siDistance = iso( l_v3Position );	// Distance vers objet  
		l_v3Position = l_v3Position + l_siDistance.fDistance * p_v3Direction;  
		l_iStep++;  
	}
	while( l_siDistance.fDistance < g_iEpsilon && l_iStep < g_iMaxStep );
	
	if( l_siDistance.fDistance < g_iEpsilon )
	{
		vec3 l_v3Normal = normalize( normal( l_v3Position, 0.05 ) );
		gl_FragColor = l_siDistance.sphere.v4Diffuse;
	}
}

void main()
{
	g_sphere1.fRayon 		= 0.5;
	g_sphere1.v4Diffuse 	= vec4( 0.5, 0.5, 1.0, 1.0 );
	g_sphere1.v4Specular	= vec4( 0.5, 0.5, 1.0, 1.0 );
	g_sphere1.fShininess	= 50.0;

	g_sphere2.fRayon 		= 0.2;
	g_sphere2.v4Diffuse 	= vec4( 1.0, 0.5, 0.5, 1.0 );
	g_sphere2.v4Specular	= vec4( 1.0, 0.5, 0.5, 1.0 );
	g_sphere2.fShininess	= 10.0;

	vec3 l_v3CameraPosition = vec3( gl_FragCoord.x, gl_FragCoord.y, 0.0 );
	vec3 l_v3CameraDirection = vec3( 0.0, 0.0, 1.0 );
	
	ray( l_v3CameraPosition, l_v3CameraDirection );
}