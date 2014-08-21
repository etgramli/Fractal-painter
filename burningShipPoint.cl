/* burningShipPoint.cl
 * Calculates the mandelbrot value
 * for a certain pixel / point
 */

__kernel void burningShipPoint(__write_only image2d_t outImg){
	int2 coord = {get_global_id(0), get_global_id(1)};
	int2 range = {get_global_size(0), get_global_size(1)};
	float4 color = (float4)0.0f;
	float2 c, p, p0 = (float2)(0.0f);	// Contains complex numbers
    float imgRange = 3.0f;
	c.x = 2.0f * imgRange * (coord.x / (float)range.x - 0.5f);
	c.y = 2.0f * imgRange * (coord.y / (float)range.y - 0.5f);
	
    for(int l = 0; l < 360; l++){
        p = (float2)( (p0.x < 0)? -p0.x : p0.x,
					  (p0.y < 0)? -p0.y : p0.y);
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
