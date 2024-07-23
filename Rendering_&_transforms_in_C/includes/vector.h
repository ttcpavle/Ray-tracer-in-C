#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector3 {
	float x, y, z;
}Vector3;

// constants
extern const Vector3 world_up;
extern const Vector3 vector_zero;

typedef struct Coordinate {
	float x, y;
}Coordinate;

typedef struct Quaternion {
	float w, x, y, z;
}Quaternion;

typedef struct Euler_Angles {
	float roll, pitch, yaw;
}Euler_Angles;

// Basic vector operations:
float dot(Vector3 v1, Vector3 v2);
Vector3 cross(struct Vector3 v1, struct Vector3 v2);
Vector3 vector_add(Vector3 v1, Vector3 v2);
Vector3 vector_sub(Vector3 v1, Vector3 v2);
float length(Vector3 v);
float distance(Vector3 vertex1, Vector3 vertex2);
Vector3 scale(Vector3 v, float t);
Vector3 normalize(Vector3 v);
Vector3 negate(Vector3 v);

// Quaternion operations:
Quaternion quaternion_from_axis_angle(Vector3 axis, float angle);
Quaternion quaternion_normalize(Quaternion q);
Quaternion quaternion_multiply(Quaternion q1, Quaternion q2);

#endif //VECTOR_H