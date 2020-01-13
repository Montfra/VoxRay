struct Ray{
	float3 origin;
	float3 dir;
};

struct Sphere{
	float radius;
	float3 pos;
	float3 emi;
	float3 color;
};

struct Cube {
	float3 pos;
};

bool intersect_cube(const struct Cube* cube, const struct Ray* r) {

	float3 lb = (float3)( cube->pos.x, cube->pos.y -0.3f, cube->pos.z -20.0f);
	float3 rt = (float3)( cube->pos.x + 0.3f,  cube->pos.y, cube->pos.z);

	float3 dirfrac;
	
	dirfrac.x = 1.0f / r->dir.x;
	dirfrac.y = 1.0f / r->dir.y;
	dirfrac.z = 1.0f / r->dir.z;
	
	float t1 = (lb.x - r->origin.x) * dirfrac.x;
	float t2 = (rt.x - r->origin.x) * dirfrac.x;
	float t3 = (lb.y - r->origin.y) * dirfrac.y;
	float t4 = (rt.y - r->origin.y) * dirfrac.y;
	float t5 = (lb.z - r->origin.z) * dirfrac.z;
	float t6 = (rt.z - r->origin.z) * dirfrac.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	
	if (tmax < 0.0f)
	{
		
		return false;
	}

	
	if (tmin > tmax)
	{
		
		return false;
	}

	
	return true;
}

bool intersect_sphere(const struct Sphere* sphere, const struct Ray* ray, float* t)
{
	float3 rayToCenter = sphere->pos - ray->origin;

	/* calculate coefficients a, b, c from quadratic equation */

	/* float a = dot(ray->dir, ray->dir); // ray direction is normalised, dotproduct simplifies to 1 */
	float b = dot(rayToCenter, ray->dir);
	float c = dot(rayToCenter, rayToCenter) - sphere->radius*sphere->radius;
	float disc = b * b - c; /* discriminant of quadratic formula */

	/* solve for t (distance to hitpoint along ray) */

	if (disc < 0.0f) return false;
	else *t = b - sqrt(disc);

	if (*t < 0.0f){
		*t = b + sqrt(disc);
		if (*t < 0.0f) return false;
	}

	else return true;
}

int tclam(float s) { if (s > 255.0f) { return 255; } if (s < 0.0f) { return 0; } return s; }
int toInt(float x) { return tclam(x * 255); }

unsigned int rgb(int r, int g, int b) {
	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + ((255 & 0xff) << 0);
}

struct Ray createCamRay(const int x_coord, const int y_coord, const int width, const int height){

	float fx = (float)x_coord / (float)width;  /* convert int in range [0 - width] to float in range [0-1] */
	float fy = (float)y_coord / (float)height; /* convert int in range [0 - height] to float in range [0-1] */

	/* calculate aspect ratio */
	float aspect_ratio = (float)(width) / (float)(height);
	float fx2 = (fx - 0.5f) * aspect_ratio;
	float fy2 = fy - 0.5f;

	/* determine position of pixel on screen */
	float3 pixel_pos = (float3)(fx2, -fy2, 0.0f);

	/* create camera ray*/
	struct Ray ray;
	ray.origin = (float3)(0.0f, 0.0f, 40.0f); /* fixed camera position */
	ray.dir = normalize(pixel_pos - ray.origin); /* ray direction is vector from camera to pixel */

	return ray;
}

__kernel void render_kernel(int width, int height, int rendermode,__global unsigned int* pix)
{

	const int work_item_id = get_global_id(0);		/* the unique global id of the work item for the current pixel */
	
	int x_coord = work_item_id % width;					/* x-coordinate of the pixel */
	int y_coord = work_item_id / width;					/* y-coordinate of the pixel */

	float fx = (float)x_coord / (float)width;  /* convert int in range [0 - width] to float in range [0-1] */
	float fy = (float)y_coord / (float)height; /* convert int in range [0 - height] to float in range [0-1] */

	/*create a camera ray */
	struct Ray camray = createCamRay(x_coord, y_coord, width, height);

	

	/* create and initialise a sphere */
	struct Sphere sphere1;
	float xxx = (((float)rendermode+80) / 1000.0f)/2;
	sphere1.radius = xxx;
	sphere1.pos = (float3)(-0.5f, 0.0f, 3.0f);
	sphere1.color = (float3)((400 - xxx)/400, 0.6f, xxx/400);

	/* create and initialise a sphere2 */
    	struct Sphere sphere2;
    	sphere2.radius = 0.3 - xxx;
    	sphere2.pos = (float3)(0.5f, 0.0f, 3.0f);
    	sphere2.color = (float3)(1 - fy * 0.1f, 1 - fy * 0.3f, 1 - 0.3f);

		struct Cube cube;
		cube.pos = (float3)(0.3 - xxx, -0.5f, -50.0f);

	rendermode = 3;
	/* intersect ray with sphere */
	float t = 1e20;
	intersect_sphere(&sphere1, &camray, &t);

	/* intersect ray with sphere2 */
    float t2 = 1e20;
    intersect_sphere(&sphere2, &camray, &t2);

	/* if ray misses sphere, return background colour
	background colour is a blue-ish gradient dependent on image height */
	if (t > 1e19 && t2 > 1e19 && rendermode != 1){
		pix[work_item_id] = rgb(toInt(fy * 0.1f), toInt(fy * 0.3f), toInt(0.3f));

		if (intersect_cube(&cube, &camray))
		{
			pix[work_item_id] = rgb(toInt(0.1f), toInt(0.3f), toInt(0.3f));
		}

		return;
	}

	if (intersect_cube(&cube, &camray))
	{
		pix[work_item_id] = rgb(toInt(0.1f), toInt(0.3f), toInt(0.3f));
	}

	/* for more interesting lighting: compute normal
	and cosine of angle between normal and ray direction */
	float3 hitpoint = camray.origin + camray.dir * t;
	float3 normal = normalize(hitpoint - sphere1.pos);
	float cosine_factor = dot(normal, camray.dir) * -1.0f;

	/* for more interesting lighting: compute normal
    	and cosine of angle between normal and ray direction */
    	float3 hitpoint2 = camray.origin + camray.dir * t2;
    	float3 normal2 = normalize(hitpoint2 - sphere2.pos);
    	float cosine_factor2 = dot(normal2, camray.dir) * -1.0f;

    pix[work_item_id] = rgb(toInt((sphere1.color * cosine_factor).x), toInt((sphere1.color * cosine_factor).y), toInt((sphere1.color * cosine_factor).z));

	/* six different rendermodes */
	if (rendermode == 1) pix[work_item_id] = rgb(toInt(((float3)(fx, fy, 0)).x), toInt(((float3)(fx, fy, 0)).y), toInt(((float3)(fx, fy, 0)).z));
	if (t <= 1e19 && rendermode == 2) pix[work_item_id] = rgb(toInt((sphere1.color).x), toInt((sphere1.color).y), toInt((sphere1.color).z));
	if (t2 <= 1e19 && rendermode == 2) pix[work_item_id] = rgb(toInt((sphere2.color).x), toInt((sphere2.color).y), toInt((sphere2.color).z));
	if (t <= 1e19 && rendermode == 3) pix[work_item_id] = rgb(toInt((sphere1.color * cosine_factor).x), toInt((sphere1.color * cosine_factor).y), toInt((sphere1.color * cosine_factor).z));
	if (t2 <= 1e19 && rendermode == 3) pix[work_item_id] = rgb(toInt((sphere2.color * cosine_factor2).x), toInt((sphere2.color * cosine_factor2).y), toInt((sphere2.color * cosine_factor2).z));
}