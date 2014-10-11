/* mandelbrotPoint.cl
 * Calculates the mandelbrot value
 * for a certain pixel / point
 */

__kernel void mandelbrotPoint(__write_only image2d_t outImg,
                              float range, float midX, float midY){
	int2 coord = {get_global_id(0), get_global_id(1)};
	int2 res = {get_global_size(0), get_global_size(1)};
	float4 color = (float4)0.0f;
	float2 c, z = (float2)(0.0f);	// Contains complex numbers
	
	c.x = 1.5f * range * (coord.x / (float)res.x - 0.5f) + midX;
	c.y = 1.0f * range * (coord.y / (float)res.y - 0.5f) + midY;
	
	float xsqr = z.x * z.x;
	float ysqr = z.y * z.y;
	
    for(int l = 0; l < 360; l++){
		z.y = z.x * z.y;
		z.y += z.y;
		z.y += c.y;
        z.x = xsqr - ysqr + c.x;
        if(xsqr + ysqr > 4.0f){
			color = (float)l / (float)360;
			write_imagef(outImg, coord, color);
            return;
		}
		xsqr = z.x * z.x;
		ysqr = z.y * z.y;
    }
	color = 0.0f;
    write_imagef(outImg, coord, color);
}
