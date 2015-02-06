#define KERNEL_NAME( name ) name
#define KERNEL_PARAM( name ) name

__kernel void KERNEL_NAME( Kernel )( __read_only  image2d_t KERNEL_PARAM( pg_srcImg ), __write_only image2d_t KERNEL_PARAM( pg_dstImg ), int KERNEL_PARAM( pg_sep_type ), int KERNEL_PARAM( pg_sep_offset ) )
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
