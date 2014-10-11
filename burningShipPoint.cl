/* burningShipPoint.cl
 * Calculates the mandelbrot value
 * for a certain pixel / point
 */

__kernel void burningShipPoint(__write_only image2d_t outImg,
                               float range, float midX, float midY){
	int2 coord = {get_global_id(0), get_global_id(1)};
	int2 res = {get_global_size(0), get_global_size(1)};
	float4 color = (float4)0.0f;
	float2 c, p, p0 = (float2)(0.0f);	// Contains complex numbers

	c.x = 2.0f * range * (coord.x / (float)res.x - 0.5f) + midX;
	c.y = 2.0f * range * (coord.y / (float)res.y - 0.5f) + midY;

    p = fabs(p);
	float xsqr = p.x * p.x;
	float ysqr = p.y * p.y;
    for(int l = 0; l < 360; l++){
		p.y = p.x * p.y;
		p.y += p.y;
		p.y += c.y;
        p.x = xsqr - ysqr + c.x;

        if(xsqr + ysqr > 10.0f){
			color = (float)l / (float)360;
			write_imagef(outImg, coord, color);
            return;
		}
        p = fabs(p);
		xsqr = p.x * p.x;
		ysqr = p.y * p.y;
    }
	color = 0.0f;
    write_imagef(outImg, coord, color);
}
