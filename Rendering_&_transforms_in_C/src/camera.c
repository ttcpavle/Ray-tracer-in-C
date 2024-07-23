#include "../includes/config.h"
#include "../includes/vector.h"
#include "../includes/transformations.h"
#include "../includes/camera.h"

// from: https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function/framing-lookat-function.html
static t_matrix calc_cam_to_world(Vector3 eye, Vector3 look_at, Vector3 up) {
    Vector3 forward = normalize(vector_sub(look_at, eye));
    Vector3 right = normalize(cross(forward, up));
    Vector3 camera_up = cross(right, forward);

    t_matrix m = identity();
    m.matrix[0][0] = right.x;
    m.matrix[0][1] = right.y;
    m.matrix[0][2] = right.z;

    m.matrix[1][0] = camera_up.x;
    m.matrix[1][1] = camera_up.y;
    m.matrix[1][2] = camera_up.z;

    m.matrix[2][0] = -forward.x;
    m.matrix[2][1] = -forward.y;
    m.matrix[2][2] = -forward.z;

    // i used eye coordinates as last column because of translation
    m.matrix[0][3] = eye.x;
    m.matrix[1][3] = eye.y;
    m.matrix[2][3] = eye.z;
    return m;
}

void Set_Camera(Camera* camera, Vector3 look_at, Vector3 up, Vector3 eye, float fov, float aspect_ratio) {
    camera->eye = eye;
    camera->look_at = look_at;
    camera->fov = fov;
    camera->aspect_ratio = aspect_ratio;
    camera->up = up;
    camera->camera_to_world = calc_cam_to_world(eye, look_at, up);
    camera->world_to_camera = inverse(camera->camera_to_world);
    printf("Camera set up sucessfully\n");

}