varying vec2 pxl_texture;

uniform sampler2D pg_texture;
uniform int pg_time;
uniform int pg_width;
uniform int pg_height;

void main()
{
	float time = float( pg_time) / 1000.0;
	vec3 shockParams = vec3( 10.0, 0.8, 0.1);
	vec2 center = vec2( (float( pg_width) / 2.0) / float( pg_width), (float( pg_height) / 2.0) / float( pg_height));
	vec2 uv = pxl_texture;
	vec2 texCoord = uv;
	float distance = distance(uv, center);
	if ( (distance <= (time + shockParams.z)) && (distance >= (time - shockParams.z)) )
	{
		float diff = (distance - time);
		float powDiff = 1.0 - pow(abs(diff*shockParams.x), shockParams.y);
		float diffTime = diff  * powDiff;
		vec2 diffUV = normalize(uv - center);
		texCoord = uv + (diffUV * diffTime);
	}
	gl_FragColor = texture2D(pg_texture, texCoord);
}