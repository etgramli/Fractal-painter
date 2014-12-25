/* julia.cl
 * OpenCL Kernel that computes the
 * julia value for a certain pixel / point.
 */

__kernel void juliaPoint(__constant const float2 *c,
						 __write_only image2d_t outImg,
                         float range, float midX, float midY){
	int2 coord = (int2) (get_global_id(0), get_global_id(1));
	int2 res = {get_global_size(0), get_global_size(1)};
	float2 z = (float2)(0.0f);

	z.x = range * (coord.x / (float)res.x - 0.5f) + midX;
    z.y	= range * (coord.y / (float)res.y - 0.5f) + midY;
	
    // Calcuate max. interations depending on the level of zoom
    int maxiter = 360;
    if(range > 0.0036)
        maxiter = 360 - (float)140*range;

	float xsqr = z.x * z.x;
	float ysqr = z.y * z.y;
    for(int l = 0; l < maxiter; l++){
		z.y = z.x * z.y;
		z.y += z.y;
		z.x = xsqr - ysqr;
		z += *c;
        if(xsqr + ysqr > 1000){
			write_imagef(outImg, coord, (float)l / maxiter);
            return;
        }
		xsqr = z.x * z.x;
		ysqr = z.y * z.y;
    }
    write_imagef(outImg, coord, 0.0f);
}
