struct Ray{
	float3 origin;
	float3 dir;
};

struct Triangle {
	float3 pos;
	float3 color;

	float3 p1;
	float3 p2;
	float3 p3;
};

struct Sphere{
	float radius;
	float3 pos;
	float3 emi;
	float3 color;
};

struct Light {
	float3 pos;
};

struct Cube {
	float* vertices;
	float3 pos;
	float3 color;
};



bool intersect_triangle(const struct Triangle* t, const struct Ray* r, float* tnear) {
	float3 edge1 = t->p2 - t->p1;
	float3 edge2 = t->p3 - t->p1;
	float3 pvec = cross(r->dir, edge2);
	float det = dot(edge1, pvec);

	if (det < 1e-5) return false;

	float3 tvec = r->origin - t->p1;
	float u = dot(tvec, pvec);

	if (u < 0 || u > det) return false;

	float3 qvec = cross(tvec, edge1);
	float v = dot(r->dir, qvec);
	if (v < 0 || u + v > det) return false;

	*tnear = dot(edge2, qvec) * (1. / det);
	return *tnear > 1e-5;
}



bool intersect_cube(const struct Cube* cube, const struct Ray* r, float* t) {

	float3 lb = (float3)( cube->pos.x, cube->pos.y -0.05f, cube->pos.z -0.8f);
	float3 rt = (float3)( cube->pos.x + 0.05f,  cube->pos.y, cube->pos.z);

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
		*t = tmax;
		return false;
	}

	
	if (tmin > tmax)
	{
		*t = tmax;
		return false;
	}


	*t = tmin;
	
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



	float dir_x = (x_coord + 0.5) - width / 2.0f;
	float dir_y = -(y_coord + 0.5) + height / 2.0f;   
	float dir_z = -height / (2.0f * tan((3.14/3.0f) / 2.0f));

	/* create camera ray*/
	struct Ray ray;
	ray.origin = (float3)(0.0f, 0.0f, -40.0f); /* fixed camera position */
	ray.dir = normalize(pixel_pos - ray.origin); /* ray direction is vector from camera to pixel */

	return ray;
}

void setPosition(float* model, float3 position) {
	for (int i = 0; i < 108; i += 9) {
		model[i] += position.x;
		model[i + 1] += position.y;
		model[i + 2] += position.z;

		model[i + 3] += position.x;
		model[i + 4] += position.y;
		model[i + 5] += position.z;

		model[i + 6] += position.x;
		model[i + 7] += position.y;
		model[i + 8] += position.z;
	}
}

__kernel void render_kernel(int width, int height, int rendermode, __global unsigned int* pix, __global float* model)
{

	float mdl[108];

	/* FRONT */
	mdl[0] = -0.5f;
	mdl[1] = 0.3f;
	mdl[2] = 2.0f;

	mdl[3] = 0.5f;
	mdl[4] = 0.3f;
	mdl[5] = 2.0f;

	mdl[6] = -0.5f;
	mdl[7] = -0.2f;
	mdl[8] = 2.0f;

	mdl[9] = 0.5f;
	mdl[10] = 0.3f;
	mdl[11] = 2.0f;

	mdl[12] = 0.5f;
	mdl[13] = -0.2f;
	mdl[14] = 2.0f;

	mdl[15] = -0.5f;
	mdl[16] = -0.2f;
	mdl[17] = 2.0f;

	/* Normal (0.0f, 0.0f, -1.0f) */

	/* TOP */
	mdl[18] = 0.5f;
	mdl[19] = 0.3f;
	mdl[20] = 10.0f;

	mdl[21] = 0.5f;
	mdl[22] = 0.3f;
	mdl[23] = 2.0f;

	mdl[24] = -0.5f;
	mdl[25] = 0.3f;
	mdl[26] = 2.0f;

	mdl[27] = -0.5f;
	mdl[28] = 0.3f;
	mdl[29] = 10.0f;

	mdl[30] = 0.5f;
	mdl[31] = 0.3f;
	mdl[32] = 10.0f;

	mdl[33] = -0.5f;
	mdl[34] = 0.3f;
	mdl[35] = 2.0f;

	/* Normal (0.0f, 1.0f, 0.0f) */


	/* LEFT */
	mdl[36] = -0.5f;
	mdl[37] = 0.3f;
	mdl[38] = 2.0f;

	mdl[39] = -0.5f;
	mdl[40] = -0.2f;
	mdl[41] = 2.0f;

	mdl[42] = -0.5f;
	mdl[43] = 0.3f;
	mdl[44] = 10.0f;

	mdl[45] = -0.5f;
	mdl[46] = 0.3f;
	mdl[47] = 10.0f;

	mdl[48] = -0.5f;
	mdl[49] = -0.2f;
	mdl[50] = 2.0f;

	mdl[51] = -0.5f;
	mdl[52] = -0.2f;
	mdl[53] = 10.0f;

	/* Normal (-1.0f, 0.0f, 0.0f) */


	/* RIGHT */
	mdl[54] = 0.5f;
	mdl[55] = 0.3f;
	mdl[56] = 10.0f;

	mdl[57] = 0.5f;
	mdl[58] = -0.2f;
	mdl[59] = 2.0f;

	mdl[60] = 0.5f;
	mdl[61] = 0.3f;
	mdl[62] = 2.0f;

	mdl[63] = 0.5f;
	mdl[64] = -0.2f;
	mdl[65] = 10.0f;

	mdl[66] = 0.5f;
	mdl[67] = -0.2f;
	mdl[68] = 2.0f;

	mdl[69] = 0.5f;
	mdl[70] = 0.3f;
	mdl[71] = 10.0f;

	/* Normal (1.0f, 0.0f, 0.0f) */

	/* BOTTOM */
	mdl[72] = -0.5f;
	mdl[73] = -0.2f;
	mdl[74] = 2.0f;

	mdl[75] = 0.5f;
	mdl[76] = -0.2f;
	mdl[77] = 2.0f;

	mdl[78] = 0.5f;
	mdl[79] = -0.2f;
	mdl[80] = 10.0f;

	mdl[81] = -0.5f;
	mdl[82] = -0.2f;
	mdl[83] = 2.0f;

	mdl[84] = 0.5f;
	mdl[85] = -0.2f;
	mdl[86] = 10.0f;

	mdl[87] = -0.5f;
	mdl[88] = -0.2f;
	mdl[89] = 10.0f;

	/* Normal (0.0f, -1.0f, 0.0f) */


	/* BACK */
	mdl[90] = -0.5f;
	mdl[91] = 0.3f;
	mdl[92] = 10.0f;

	mdl[93] = 0.5f;
	mdl[94] = 0.3f;
	mdl[95] = 10.0f;

	mdl[96] = -0.5f;
	mdl[97] = -0.2f;
	mdl[98] = 10.0f;

	mdl[99] = 0.5f;
	mdl[100] = 0.3f;
	mdl[101] = 10.0f;

	mdl[102] = 0.5f;
	mdl[103] = -0.2f;
	mdl[104] = 10.0f;

	mdl[105] = -0.5f;
	mdl[106] = -0.2f;
	mdl[107] = 10.0f;

	/* Normal (0.0f, 0.0f, 1.0f) */
	
	const int work_item_id = get_global_id(0);		/* the unique global id of the work item for the current pixel */

	int x_coord = work_item_id % width;					/* x-coordinate of the pixel */
	int y_coord = work_item_id / width;					/* y-coordinate of the pixel */

	float fx = (float)x_coord / (float)width;  /* convert int in range [0 - width] to float in range [0-1] */
	float fy = (float)y_coord / (float)height; /* convert int in range [0 - height] to float in range [0-1] */

	float xxx = (((float)rendermode) / 10.0f);
	/*create a camera ray */
	struct Ray camray = createCamRay(x_coord, y_coord, width, height);

	struct Light light;
	light.pos = (float3)(xxx, 0.2f, 30.0f);

	/* create and initialise a sphere */
	struct Sphere sphere1;
	sphere1.radius = 0.09f;
	sphere1.pos = (float3)(xxx, 0.2f, 31.0f);
	sphere1.color = (float3)((400 - xxx)/400, 0.6f, xxx/400);

	/* create and initialise a sphere2 */
    	struct Sphere sphere2;
    	sphere2.radius = 0.2f;
    	sphere2.pos = (float3)(0.6f, 0.0f, -500.0f);
    	sphere2.color = (float3)(0.1f, 0.3f, 0.3f);

	rendermode = 3;
	/* intersect ray with sphere */
	float t = 1e20;
	intersect_sphere(&sphere1, &camray, &t);

	/* intersect ray with sphere2 */
    float t2 = 1e20;
    intersect_sphere(&sphere2, &camray, &t2);

	float t3 = 1e20;

	float tnear;
	
	bool alreadyColored = true;

	for (int i = 0; i < 9; i+=3) {

		struct Cube cc;

		if (i < 4)
		{
			cc.pos = (float3)(xxx/10.0f, xxx / 10.0f, 300.0f);
		}
		else
		{
			cc.pos = (float3)(model[0], model[1], model[2]);
		}

		
		cc.color = (float3)(xxx / 10.0f, 0.4f, xxx / 10.0f);
		setPosition(cc.vertices, cc.pos);
		cc.vertices = &mdl;

		
		


		for (int i = 0; i < 109; i += 9) {
			struct Triangle tr;
			tr.pos = (float3)(0.0f, 0.0f, 0.0f);
			tr.color = cc.color;
			tr.p1 = (float3)(cc.vertices[i], cc.vertices[i + 1], cc.vertices[i + 2]);
			tr.p2 = (float3)(cc.vertices[i + 3], cc.vertices[i + 4], cc.vertices[i + 5]);
			tr.p3 = (float3)(cc.vertices[i + 6], cc.vertices[i + 7], cc.vertices[i + 8]);


			if (intersect_triangle(&tr, &camray, &tnear)) {

				float3 normal = (float3)(1.0f, 0.0f, 0.0f);
				if (i >= 0 && i <= 17)
				{
					normal = (float3)(0.0f, 0.0f, -1.0f);

				}
				else if (i >= 18 && i <= 35)
				{
					normal = (float3)(0.0f, 1.0f, 0.0f);

				}
				else if (i >= 36 && i <= 53)
				{
					normal = (float3)(-1.0f, 0.0f, 0.0f);

				}
				else if (i >= 54 && i <= 71)
				{
					normal = (float3)(1.0f, 0.0f, 0.0f);

				}
				else if (i >= 72 && i <= 89)
				{
					normal = (float3)(0.0f, -1.0f, 0.0f);

				}
				else if (i >= 90 && i <= 107)
				{
					normal = (float3)(0.0f, 0.0f, 1.0f);

				}


				float3 hitpoint3 = camray.origin + camray.dir * tnear;

				float cosine_factor3 = (dot(normal, normalize(hitpoint3 - light.pos)) * -1.0f) + 0.5f;

				float specular = 0 * pow(cosine_factor3, 2);

				pix[work_item_id] = rgb(toInt((tr.color * cosine_factor3 + tr.color * specular).x), toInt((tr.color * cosine_factor3 + tr.color * specular).y), toInt((tr.color * cosine_factor3 + tr.color * specular).z));
				alreadyColored = false;
				break;
				
			}
		}
		setPosition(cc.vertices, -cc.pos);
		
	}

	if (alreadyColored) {
		pix[work_item_id] = rgb(toInt(0.1f), toInt(0.3f), toInt(0.3f));
	}

	if (t > 1e19 && rendermode != 1) { return; }

	/* if ray misses sphere, return background colour
	background colour is a blue-ish gradient dependent on image height 

		pix[work_item_id] = rgb(toInt(0.1f), toInt(0.3f), toInt(0.3f));

		float x = -1.0f;
		float y = 0.0f;

		for (size_t i = 0; i < 400; i++)
		{
			struct Cube cube;
			x += 0.06f;
			

			if (x >= 1.0f)
			{
				x = -1.0f;
				y -= 0.1f;
			}

			cube.pos = (float3)(x, y, 0.3f);
			cube.color = (float3)(x, 0.4f, 0.8f);

			if (intersect_cube(&cube, &camray, &t3))
			{
				float3 hitpoint3 = camray.origin + camray.dir * t3;
				float3 normal3 = normalize(hitpoint3 - cube.pos);
				normal3.z = normal3.z * -1.0f;
				
				float cosine_factor3 = (dot(normal3, normalize(hitpoint3 - light.pos)) * -1.0f) + 0.5f;
				pix[work_item_id] = rgb(toInt((cube.color * cosine_factor3).x), toInt((cube.color * cosine_factor3).y), toInt((cube.color * cosine_factor3).z));
				return;
			}
		}

		return;
	*/

	

	/* for more interesting lighting: compute normal
	and cosine of angle between normal and ray direction */
	float3 hitpoint = camray.origin + camray.dir * t;
	float3 normal = normalize(hitpoint - sphere1.pos);
	float cosine_factor = (dot(normal, normalize(hitpoint - light.pos)) * -1.0f) + 0.5f;

	/* for more interesting lighting: compute normal
    	and cosine of angle between normal and ray direction */
    	float3 hitpoint2 = camray.origin + camray.dir * t2;
    	float3 normal2 = normalize(hitpoint2 - sphere2.pos);
    	float cosine_factor2 = (dot(normal2, normalize(hitpoint2 - light.pos)) * -1.0f) + 0.5f;

    pix[work_item_id] = rgb(toInt((sphere1.color * cosine_factor).x), toInt((sphere1.color * cosine_factor).y), toInt((sphere1.color * cosine_factor).z));

	/*
	if (rendermode == 1) pix[work_item_id] = rgb(toInt(((float3)(fx, fy, 0)).x), toInt(((float3)(fx, fy, 0)).y), toInt(((float3)(fx, fy, 0)).z));
	if (t <= 1e19 && rendermode == 2) pix[work_item_id] = rgb(toInt((sphere1.color).x), toInt((sphere1.color).y), toInt((sphere1.color).z));
	if (t2 <= 1e19 && rendermode == 2) pix[work_item_id] = rgb(toInt((sphere2.color).x), toInt((sphere2.color).y), toInt((sphere2.color).z));
	if (t <= 1e19 && rendermode == 3) pix[work_item_id] = rgb(toInt((sphere1.color * cosine_factor).x), toInt((sphere1.color * cosine_factor).y), toInt((sphere1.color * cosine_factor).z));
	if (t2 <= 1e19 && rendermode == 3) pix[work_item_id] = rgb(toInt((sphere2.color * cosine_factor2).x), toInt((sphere2.color * cosine_factor2).y), toInt((sphere2.color * cosine_factor2).z));
	 */
}