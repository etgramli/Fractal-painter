/* julia.cl
 * OpenCL Kernel that computes the
 * julia value for a certain pixel / point.
 */

__kernel void juliaPoint(__constant const float2 *c,
						 __write_only image2d_t outImg){
	int2 coord = (int2) (get_global_id(0), get_global_id(1));
	int2 range = {get_global_size(0), get_global_size(1)};
	float2 tmp, z = (float2)(0.0f);
	z.x = (float)(range.x / 2.0 - coord.x) / (range.x / 2.0f);
    z.y	= (float)(range.y / 2.0 - coord.y) / (range.y / 2.0f);
	
	int l;
    for(l = 0; l < 360; l++){
		tmp.x = z.x*z.x - z.y*z.y;
		tmp.y = z.y * z.x * 2;
        z = tmp + *c;
        if(dot(z,z) > 1000){
			write_imagef(outImg, coord, (float)l / 360);
            return;
        }
    }
    write_imagef(outImg, coord, 0.0f);
}
