/* HSVtoRGB.cl
 * Hue = [0;6] = 6 * fracValue
 * Saturation = 1.0f
 * Value = [0|1] = fracValue==0 ? 0:1
 * (green, red, alpha channel, blue) x,y,z,w
 * Weird, but has to be like that because of Qt interop.
 */
__kernel void HSVtoRGB(__read_only image2d_t inImg,
					   __write_only image2d_t outImg)
{
	const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
							  CLK_ADDRESS_CLAMP |
							  CLK_FILTER_NEAREST;
	int2 coord = {get_global_id(0), get_global_id(1)};
	float fracValue = read_imagef(inImg, sampler, coord).w;
	float h = fracValue * 6.0f;
	float s = 1.0f;
	float v = (fracValue==0.0f) ? 0:1;
	uint4 pixelValue = 255;
    float f, p, q, t;
    int i = floor( h );
    f = h - i;			// factorial part of h
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    switch( i ) {
        case 0:
            pixelValue.y = v * 255;
            pixelValue.x = t * 255;
            pixelValue.w = p * 255;
            break;
        case 1:
            pixelValue.y = q * 255;
            pixelValue.x = v * 255;
            pixelValue.w = p * 255;
            break;
        case 2:
            pixelValue.y = p * 255;
            pixelValue.x = v * 255;
            pixelValue.w = t * 255;
            break;
        case 3:
            pixelValue.y = p * 255;
            pixelValue.x = q * 255;
            pixelValue.w = v * 255;
            break;
        case 4:
            pixelValue.y = t * 255;
            pixelValue.x = p * 255;
            pixelValue.w = v * 255;
            break;
        default:		// case 5:
            pixelValue.y = v * 255;
            pixelValue.x = p * 255;
            pixelValue.w = q * 255;
            break;
    }
	write_imageui(outImg, coord, pixelValue);
    return;
}