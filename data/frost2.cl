#define KERNEL_NAME( name ) name
#define KERNEL_PARAM( name ) name

float semi_rand( float x, float y )
{
	float2 co = (float2)( x, y );
	float rnd_scale = 5.1f;
	float result;
	float2 v1 = (float2)(92.0f, 80.0f);
	float2 v2 = (float2)(41.0f, 62.0f);
	return fract( sin( dot( co, v1 ) ) + cos( dot( co, v2 ) ) * rnd_scale, &result );
}

float4 ComputeColour( __read_only image2d_t srcImg, const sampler_t smp, float2 fCoord )
{
	float rnd_factor = 0.05f;
    float2 rnd = (float2)(semi_rand( fCoord.x, fCoord.y ), semi_rand( fCoord.y, fCoord.x ) );
    return read_imagef( srcImg, smp, fCoord + rnd * rnd_factor );
}

float4 ApplyColour( __read_only image2d_t srcImg, const sampler_t smp, float2 fCoord, int sep_type, int sep_offset )
{
	float4 tc = (float4)( 0.0f, 0.0f, 1.0f, 1.0f );

	if (sep_type == 0)
	{
		tc = ComputeColour( srcImg, smp, fCoord );
	}
	else if (sep_type == 1)
	{
		if (fCoord.x < (((float) sep_offset) / 100.0) - 0.005)
		{
			tc = ComputeColour( srcImg, smp, fCoord );
		}
		else if (fCoord.x > (((float) sep_offset) / 100.0) + 0.005)
		{
			tc = read_imagef( srcImg, smp, fCoord );
		}
	}
	else if (sep_type == 2)
	{
		if (fCoord.y < (((float) sep_offset) / 100.0) - 0.005)
		{
			tc = ComputeColour( srcImg, smp, fCoord );
		}
		else if (fCoord.y > (((float) sep_offset) / 100.0) + 0.005)
		{
			tc = read_imagef( srcImg, smp, fCoord );
		}
	}
	
	return tc;
}

__kernel void KERNEL_NAME( FrostGlass )( __read_only image2d_t KERNEL_PARAM( pg_srcImg ), __write_only image2d_t KERNEL_PARAM( pg_dstImg ), int KERNEL_PARAM( pg_sep_type ), int KERNEL_PARAM( pg_sep_offset ) )
{
	const sampler_t smp =	CLK_NORMALIZED_COORDS_TRUE | //Normalised coordinates
							CLK_ADDRESS_CLAMP_TO_EDGE | //Clamp to zeros
							CLK_FILTER_LINEAR;
	float2 fSize = (float2)(get_image_width(pg_srcImg), get_image_height(pg_srcImg));
	int2 iCoord = (int2)(get_global_id( 0 ), get_global_id( 1 ));
	float2 fCoord = (float2)( ((float)iCoord.x) / fSize.x, ((float)iCoord.y) / fSize.y );
	write_imagef( pg_dstImg, iCoord, ApplyColour( pg_srcImg, smp, fCoord, pg_sep_type, pg_sep_offset ) );
}

__kernel void KERNEL_NAME( GrayScale )( __read_only  image2d_t KERNEL_PARAM( pg_srcImg ), __write_only image2d_t KERNEL_PARAM( pg_dstImg ), int KERNEL_PARAM( pg_sep_type ), int KERNEL_PARAM( pg_sep_offset ) )
{
	const sampler_t smp =	CLK_NORMALIZED_COORDS_FALSE | //Natural coordinates
							CLK_ADDRESS_CLAMP_TO_EDGE | //Clamp to zeros
							CLK_FILTER_LINEAR;
	int2 coord = (int2)( get_global_id( 0 ), get_global_id( 1 ));
	uint4 bgra = read_imageui( pg_srcImg, smp, coord ); //The byte order is BGRA
	float4 bgrafloat = convert_float4( bgra ) / 255.0f; //Convert to normalized [0..1] float
	//Convert RGB to luminance (make the image grayscale).
	float luminance =  sqrt( 0.241f * bgrafloat.z * bgrafloat.z + 0.691f * bgrafloat.y * bgrafloat.y + 0.068f * bgrafloat.x * bgrafloat.x );
	bgra.x = bgra.y = bgra.z = (uint)(luminance * 255.0f);
	bgra.w = 255;
	write_imageui( pg_dstImg, coord, bgra );
}

__kernel void KERNEL_NAME( Dummy )( __read_only image2d_t KERNEL_PARAM( pg_srcImg ), __write_only image2d_t KERNEL_PARAM( pg_dstImg ) )
{
	const sampler_t smp =	CLK_NORMALIZED_COORDS_FALSE | //Natural coordinates
							CLK_ADDRESS_CLAMP_TO_EDGE | //Clamp to zeros
							CLK_FILTER_LINEAR;
	int2 coord = (int2)( get_global_id( 0 ), get_global_id( 1 ));
	uint4 bgra = read_imageui( pg_srcImg, smp, coord ); //The byte order is BGRA
	write_imageui( pg_dstImg, coord, bgra );
}