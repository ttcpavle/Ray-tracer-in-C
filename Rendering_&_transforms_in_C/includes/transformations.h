#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "vector.h"

//4x4 transformation matrix
typedef struct t_matrix {
	float matrix[4][4];
}t_matrix;

t_matrix identity();

// Scale matrix
t_matrix t_scale(float factor);

// Scale on x,y or z by given factor
t_matrix t_scale_xyz(float factor_x, float factor_y, float factor_z);

// Translation matrix
t_matrix t_translate(float tx, float ty, float tz);

/*=======================================================================
 FUNCTIONS: t_rotate_x, t_rotate_y, t_rotate_z
 Rotation matrix uses trigonometric functions to apply rotation to given vector around axis(x,y or z)
 Rotation matrices are special type of orthogonal matrices with determinant -1
 MORE INFO: https://en.wikipedia.org/wiki/Rotation_matrix
=======================================================================*/
t_matrix t_rotate_x(float angle);
t_matrix t_rotate_y(float angle);
t_matrix t_rotate_z(float angle);

/* =======================================================================
 FUNCTION: quaternion_to_matrix
 Quaternions were first described by the Irish mathematician William Rowan Hamilton in 1843
 Quaternions are 4-dimensional hyper-complex numbers with a real part and i,j,k imaginary part
 Quaternions are used to perform rotation around given axis by given angle
 This function converts quaternion to rotation matrix
 MORE INFO: https://en.wikipedia.org/wiki/Quaternion
=======================================================================*/
t_matrix quaternion_to_matrix(Quaternion q);

/* =======================================================================
 FUNCTION: euler_to_matrix
 Euler angles—often referred to as roll, pitch, and yaw—play a crucial role in 3D rotations.
 Considering the order of operations, there exist a total of 9 possible Euler rotations, which can be computed using rotation matrix multiplication.
 MORE INFO: https://en.wikipedia.org/wiki/Euler_angles
=======================================================================*/
t_matrix euler_to_matrix(Euler_Angles euler_angles);

// Transpose matrix
t_matrix transpose(t_matrix m);

// Inverse matrix
t_matrix inverse(t_matrix mat);

void transform_vector(t_matrix transform, Vector3* vector);
void transform_point(t_matrix transform, Vector3* vector);

#endif // TRANSFORMATIONS_H