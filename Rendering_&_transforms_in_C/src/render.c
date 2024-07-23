#include "../includes/config.h"
#include "../includes/color.h"
#include "../includes/render.h"
#include "../includes/object.h"
#include "../includes/transformations.h"
#include "../includes/camera.h"
#include <time.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../includes/stb_image_write.h"

#define EPSILON 0.000001

//idea from:https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays.html
static void computePrimRay(int x, int y, Ray* primRay, Camera* camera) {

    float aspect_ratio = camera->aspect_ratio;
    float fov_angle = tan(camera->fov / 2 * M_PI / 180);

    // Normalized Device Coordinates (in range 0-1)
    float px_ndc = (x + 0.5) / WIDTH;
    float py_ndc = (y + 0.5) / HEIGHT;

    // Camera space
    float px = (2 * px_ndc - 1) * fov_angle * aspect_ratio;
    float py = (1 - 2 * py_ndc) * fov_angle;

    Ray ray;

    // ray in camera space
    ray.origin = vector_zero;
    ray.direction = normalize((Vector3) { px, py, -1.0f });

    // convert to ray in world space with camera_to_world matrix
    transform_point(camera->camera_to_world, &ray.origin);
    transform_vector(camera->world_to_camera, &ray.direction);

    // update primRay
    primRay->origin = ray.origin;
    primRay->direction = normalize(ray.direction);
}

// Mollder trumbore ray intersection https://www.youtube.com/watch?v=fK1RPmF_zjQ&t=425s
static int Ray_Triangle_Intersect(Vector3 v0, Vector3 v1, Vector3 v2, Ray ray, float* u, float* v, float* t) {
    Vector3 edge1 = vector_sub(v1, v0);
    Vector3 edge2 = vector_sub(v2, v0);
    Vector3 cross_rayDir_edge2 = cross(ray.direction, edge2);
    float det = dot(edge1, cross_rayDir_edge2);
    // for barycentric coordinates u,v and w, realtion u+v+w=1 has to be satisfied
#ifdef CULLING
    if (det < EPSILON)
        return 0;
    Vector3 orig_minus_v0 = vector_sub(ray.origin, v0);
    *u = dot(orig_minus_v0, cross_rayDir_edge2);
    if (*u < 0.0f || *u > det)
        return 0;

    Vector3 cross_orig_minus_v0_edge1 = cross(orig_minus_v0, edge1);
    *v = dot(ray.direction, cross_orig_minus_v0_edge1);
    if (*v < 0.0f || *u + *v > det)
        return 0;

    *t = dot(edge2, cross_orig_minus_v0_edge1);

    float inv_det = 1.0f / det;
    *u *= inv_det;
    *v *= inv_det;
    *t *= inv_det;

#else
    if (det > -EPSILON && det < EPSILON)
        return 0;

    float inv_det = 1.0f / det;
    Vector3 orig_minus_v0 = vector_sub(ray.origin, v0);
    *u = dot(orig_minus_v0, cross_rayDir_edge2) * inv_det;

    if (*u < 0.0f || *u > 1.0f)
        return 0;

    Vector3 cross_orig_minus_v0_edge1 = cross(orig_minus_v0, edge1);
    *v = dot(ray.direction, cross_orig_minus_v0_edge1) * inv_det;
    if (*v < 0.0f || *u + *v > 1.0f)
        return 0;
    *t = dot(edge2, cross_orig_minus_v0_edge1) * inv_det;
#endif
    if (*t < 0.0f) {
        fprintf(stderr, "error: t<0\n");
    }
    return 1;
}

// checks for interection for all faces in object, updates point at which object was hit, normal of hit face
// and distance between camera eye and hit point
static int Intersect(Object3D* object, Ray ray, Vector3* pHit, Vector3* nHit, float* Distance) {

    int intersect = 0;
    float t_min = INFINITY;
    for (int i = 0; i < object->num_faces; i++) {
        Vector3 v0 = object->vertices[object->faces[i].v_indices[0]-1];
        Vector3 v1 = object->vertices[object->faces[i].v_indices[1]-1];
        Vector3 v2 = object->vertices[object->faces[i].v_indices[2]-1];
        float t, u, v;
        if (Ray_Triangle_Intersect(v0, v1, v2, ray, &u, &v, &t) && t < t_min) {
            t_min = t;
            if (!object->smooth) {
                *nHit = object->normals[object->faces[i].vn_indices[0]-1];
            }
            else {
                *nHit = normalize(cross(vector_sub(v1, v0), vector_sub(v2, v0)));
            }
            pHit->x = (1 - u - v) * v0.x + u * v1.x + v * v2.x;
            pHit->y = (1 - u - v) * v0.y + u * v1.y + v * v2.y;
            pHit->z = (1 - u - v) * v0.z + u * v1.z + v * v2.z;
            intersect = 1;
        }

    }
    *Distance = t_min;
    return intersect;
}

Vector3 reflect(Vector3 I, Vector3 N) {
    return vector_sub(I, scale(N, 2.0f * dot(I, N)));
}


// Calculate color for individual pixel, shading
// Works only for single light in scene
static Color Trace(Object3D* objects[], Light lights[], int num_obj, int num_lights, Ray ray, int depth) {

    Object3D* object = NULL;
    float minDistance = INFINITY;
    Color backgroundColor = gray;
    Vector3 pHit;
    Vector3 nHit;
    Vector3 pHit_min;
    Vector3 nHit_min;
    float Distance = INFINITY;
    int index = 0;
    for (int k = 0; k < num_obj; ++k) {
        if (Intersect(objects[k], ray, &pHit, &nHit, &Distance)) {
            if (Distance < minDistance) {
                object = objects[k];
                index = k;
                pHit_min = pHit;
                nHit_min = nHit;
                minDistance = Distance;
            }
        }
    }
    if (object == NULL) 
        return backgroundColor;

    if (object->isGlass && depth <= MAX_RAY_DEPTH) {

        return blue;
    }
    else if (!object->isGlass) {

        Vector3 light_dir = vector_sub(lights[0].position, pHit_min);
        float r = length(light_dir);
        light_dir = normalize(light_dir);
        float diffuse = fmax(0.0f, dot(nHit_min, light_dir)) * lights[0].intensity;
        float attenuation = 1.0f / (r * r); // 1/(r^2)
        diffuse *= attenuation;
        diffuse = fmin(fmax(diffuse, 0.1f), 1.0f);// clamp

        return (Color)
        {
            (uint8_t)(objects[index]->color.r * diffuse),
            (uint8_t)(objects[index]->color.g * diffuse),
            (uint8_t)(objects[index]->color.b * diffuse)
        };
    }
    return backgroundColor;
}

Color* Render(Object3D* objects[], Light lights[], int num_obj, int num_lights, Camera* camera) {
    for (int i = 0; i < num_obj; i++) {
        if (objects[i] == NULL) {
            fprintf(stderr, "Invalid object in scene, render abort\n");
            return NULL;
        }
    }
    printf("\nRendering started...\n");
    clock_t begin = clock();
    Color* image = (Color*)malloc(WIDTH * HEIGHT * sizeof(Color));

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {

            Ray primRay;
            int index = y * WIDTH + x;
            if ((index % ((WIDTH * HEIGHT) / 10)) == 0 && index != 0) {
                printf("Render %d%%...\n", (100 * index) / (WIDTH * HEIGHT));
            }

            computePrimRay(x, y, &primRay, camera);
            image[index] = Trace(objects, lights, num_obj, num_lights, primRay, MAX_RAY_DEPTH);
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Render 100%%\nRender time: %.3lfs\n", time_spent);
    return image;
}
