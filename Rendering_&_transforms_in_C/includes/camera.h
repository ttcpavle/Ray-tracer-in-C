#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "transformations.h"

typedef struct Camera {
    Vector3 eye;
    Vector3 look_at;
    Vector3 up;
    float fov;
    float aspect_ratio;
    t_matrix camera_to_world;
    t_matrix world_to_camera;
}Camera;

// Set up camera
// NOTE: recommended to use WIDTH/HEIGHT from config.h as aspect_ratio
void Set_Camera(Camera* camera, Vector3 look_at, Vector3 up, Vector3 eye, float fov, float aspect_ratio);

#endif