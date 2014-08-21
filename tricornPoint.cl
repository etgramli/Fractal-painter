/* tricornPoint.cl
 * Calculates the mandelbrot value
 * for a certain pixel / point
 */

__kernel void tricornPoint(__write_only image2d_t outImg){
	int2 coord = {get_global_id(0), get_global_id(1)};
	int2 range = {get_global_size(0), get_global_size(1)};
	float4 color = (float4)0.0f;
	float2 c, tmp, z = (float2)(0.0f);	// Contains complex numbers
	c.x = 1.5f - 3*coord.x / range.x;
	c.y = 1.0f - 2*coord.y / range.y;
	
    for(int l = 0; l < 360; l++){
		z.y *= -1;
        tmp.x = z.x*z.x - z.y*z.y;
		tmp.y = z.y * z.x * 2;
		z = tmp + c;
        if(dot(z,z) > 4.0f){
			color = (float)l / (float)360;
			write_imagef(outImg, coord, color);
            return;
		}
    }
	color = 0.0f;
    write_imagef(outImg, coord, color);
}
