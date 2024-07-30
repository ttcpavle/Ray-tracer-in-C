#ifndef RENDER_H
#define RENDER_H

#include "vector.h"
#include "transformations.h"
#include "object.h"
#include "camera.h"

typedef struct {
    Vector3 origin;
    Vector3 direction;
} Ray;

typedef struct {
    Vector3 position;
    float intensity;
} Light;

typedef struct {
    Vector3 center;
    float radius;
    Color color;
} Sphere;

/* =======================================================================
 FUNCTION: Render
 Allocates memory for Color* (image as array/matrix of colors) and assigns each pixel a color computed
 with shading algorithms (currently a very simple shader)
 -set up objects, lights, camera, materials before using
 -currently supports only 1 object and 1 light in array!!!
=======================================================================*/
Color* Render(Object3D* objects[], Light lights[], int num_obj, int num_lights, Camera* camera);

#endif