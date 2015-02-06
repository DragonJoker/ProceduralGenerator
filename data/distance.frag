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

float sdTriPrism( vec3 p, vec2 n )
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
	return 0.0;
}

// Rotation/Translation
vec3 opTx( vec3 p, mat4 m )
{
	vec3 q = inverse( m ) * p;
//	return primitive(q);
	return 0.0;
}

// Scale
float opScale( vec3 p, float s )
{
//	return primitive( p / s ) * s;
	return 0.0;
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
