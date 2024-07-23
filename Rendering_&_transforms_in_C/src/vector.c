#include "../includes/config.h"
#include "../includes/vector.h"

const Vector3 world_up = { 0.0f, 1.0f, 0.0f }; //y axis is up
const Vector3 vector_zero = { 0.0f, 0.0f, 0.0f };

float dot(Vector3 v1, Vector3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 cross(Vector3 v1, Vector3 v2) {
	Vector3 result;
	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);
	return result;
}

Vector3 vector_add(Vector3 v1, Vector3 v2) {
	return (Vector3) { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vector3 vector_sub(Vector3 v1, Vector3 v2) {
	return (Vector3) { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

float length(Vector3 v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 normalize(Vector3 v) {
	float len = length(v);
	return (Vector3) { (float)v.x / len, (float)v.y / len, (float)v.z / len };
}

float distance(Vector3 vertex1, Vector3 vertex2) {
	return sqrtf((vertex1.x - vertex2.x) * (vertex1.x - vertex2.x) + (vertex1.y - vertex2.y) * (vertex1.y - vertex2.y) + (vertex1.z - vertex2.z) * (vertex1.z - vertex2.z));
}

Vector3 scale(Vector3 v, float t) {
	return (Vector3) { v.x* t, v.y* t, v.z* t };
}

Vector3 negate(Vector3 v) {
	return (Vector3) { -v.x, -v.y, -v.z };
}

Quaternion quaternion_from_axis_angle(Vector3 axis, float angle_Deg) {
	float angle = angle_Deg * M_PI / 180.0;
	float half_angle = angle / 2.0f;
	Quaternion q;
	q.w = cosf(half_angle);
	q.x = axis.x * sinf(half_angle);
	q.y = axis.y * sinf(half_angle);
	q.z = axis.z * sinf(half_angle);
	quaternion_normalize(q);

	return quaternion_normalize(q);;
}

Quaternion quaternion_normalize(Quaternion q) {
	float magnitude = sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	q.w /= magnitude;
	q.x /= magnitude;
	q.y /= magnitude;
	q.z /= magnitude;
	return q;
}

Quaternion quaternion_multiply(Quaternion q1, Quaternion q2) {
	Quaternion result;
	result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;

	return result;
}