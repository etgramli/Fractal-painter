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
    //float range = 3.0f;
	c.x = 2.0f * range * (coord.x / (float)res.x - 0.5f) + midX;
	c.y = 2.0f * range * (coord.y / (float)res.y - 0.5f) + midY;
	
    for(int l = 0; l < 360; l++){
        p = fabs(p0);
        p0.x = p.x*p.x - p.y*p.y;
		p0.y = p.y * p.x * 2;
		p0 += c;
        if(dot(p0,p0) > 10.0f){
			color = (float)l / (float)360;
			write_imagef(outImg, coord, color);
            return;
		}
    }
	color = 0.0f;
    write_imagef(outImg, coord, color);
}
