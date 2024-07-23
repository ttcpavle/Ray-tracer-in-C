#include "../includes/config.h"
#include "../includes/transformations.h"
#include "../includes/vector.h"

t_matrix identity() {
	t_matrix identity = {
		.matrix = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f } }
	};
	return identity;
}

t_matrix t_scale(float factor) {
	t_matrix scale = identity();
	scale.matrix[0][0] = factor;
	scale.matrix[1][1] = factor;
	scale.matrix[2][2] = factor;
	return scale;
}

t_matrix t_scale_xyz(float factor_x, float factor_y, float factor_z) {
	t_matrix scale = identity();
	scale.matrix[0][0] = factor_x;
	scale.matrix[1][1] = factor_y;
	scale.matrix[2][2] = factor_z;
	return scale;
}

t_matrix t_translate(float tx, float ty, float tz) {
	t_matrix translate = identity();
	translate.matrix[0][3] = tx;
	translate.matrix[1][3] = ty;
	translate.matrix[2][3] = tz;
	return translate;
}

t_matrix t_rotate_x(float angle) {
	t_matrix rotation_x = identity();
	rotation_x.matrix[1][1] = cosf(angle);
	rotation_x.matrix[1][2] = -sinf(angle);
	rotation_x.matrix[2][1] = sinf(angle);
	rotation_x.matrix[2][2] = cosf(angle);
	return rotation_x;
}

t_matrix t_rotate_y(float angle) {
	t_matrix rotation_y = identity();
	rotation_y.matrix[0][0] = cosf(angle);
	rotation_y.matrix[0][2] = sinf(angle);
	rotation_y.matrix[2][0] = -sinf(angle);
	rotation_y.matrix[2][2] = cosf(angle);
	return rotation_y;
}

t_matrix t_rotate_z(float angle) {
	t_matrix rotation_z = identity();
	rotation_z.matrix[0][0] = cosf(angle);
	rotation_z.matrix[0][1] = -sinf(angle);
	rotation_z.matrix[1][0] = sinf(angle);
	rotation_z.matrix[1][1] = cosf(angle);
	return rotation_z;
}

t_matrix mat_multiply(t_matrix m1, t_matrix m2) {
	t_matrix new_mat = identity();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				new_mat.matrix[i][j] += m1.matrix[i][k] * m2.matrix[k][j];
			}
		}
	}
	return new_mat;
}

t_matrix quaternion_to_matrix(Quaternion q) {
	t_matrix m = identity();
	float w = q.w, x = q.x, y = q.y, z = q.z;
	m.matrix[0][0] = 1 - 2 * (y * y + z * z);
	m.matrix[0][1] = 2 * (x * y - z * w);
	m.matrix[0][2] = 2 * (x * z + y * w);
	m.matrix[1][0] = 2 * (x * y + z * w);
	m.matrix[1][1] = 1 - 2 * (x * x + z * z);
	m.matrix[1][2] = 2 * (y * z - x * w);
	m.matrix[2][0] = 2 * (x * z - y * w);
	m.matrix[2][1] = 2 * (y * z + x * w);
	m.matrix[2][2] = 1 - 2 * (x * x + y * y);
	return m;
}

//ZYX
t_matrix euler_to_matrix(Euler_Angles euler_angles) {
	t_matrix m = identity();

	float roll = euler_angles.roll * M_PI / 180.0;
	float pitch = euler_angles.pitch * M_PI / 180.0;
	float yaw = euler_angles.yaw * M_PI / 180.0;

	float cr = cosf(roll);
	float sr = sinf(roll);
	float cp = cosf(pitch);
	float sp = sinf(pitch);
	float cy = cosf(yaw);
	float sy = sinf(yaw);

	m.matrix[0][0] = cy * cp;
	m.matrix[0][1] = cy * sp * sr - sy * cr;
	m.matrix[0][2] = cy * sp * cr + sy * sr;
	m.matrix[1][0] = sy * cp;
	m.matrix[1][1] = sy * sp * sr + cy * cr;
	m.matrix[1][2] = sy * sp * cr - cy * sr;
	m.matrix[2][0] = -sp;
	m.matrix[2][1] = cp * sr;
	m.matrix[2][2] = cp * cr;
	return m;
}

t_matrix transpose(t_matrix m) {
	return (t_matrix) {
		.matrix = {
			{ m.matrix[0][0], m.matrix[1][0], m.matrix[2][0], m.matrix[3][0] },
			{ m.matrix[0][1], m.matrix[1][1], m.matrix[2][1], m.matrix[3][1] },
			{ m.matrix[0][2], m.matrix[1][2], m.matrix[2][2], m.matrix[3][2] },
			{ m.matrix[0][3], m.matrix[1][3], m.matrix[2][3], m.matrix[3][3] }
		}
	};
}
//for debugging
static void print_mat(t_matrix m) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%0.4f ", m.matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

static void swap(float* a, float* b) {
	float temp = *a;
	*a = *b;
	*b = temp;
}

// Uses Gauss-Jordan method
t_matrix inverse(t_matrix mat) {

	//    (1) Set pivots to non-zero values
	t_matrix inv = identity();
	for (int j = 0; j < 4; j++) {
		if (mat.matrix[j][j] == 0.0f) {
			int max = j;//row
			for (int i = j; i < 4; i++) {
				if (fabs(mat.matrix[i][j]) >= fabs(mat.matrix[max][j])) {
					max = i;
				}
			}
			if (max == j) {
				fprintf(stderr, "Singular matrix, couldn't invert\n");
				return identity();
			}
			else {
				//switch rows
				for (int k = 0; k < 4; k++) {
					swap(&mat.matrix[max][k], &mat.matrix[j][k]);
					swap(&inv.matrix[max][k], &inv.matrix[j][k]);
				}
			}
		}
	}
	//   (2) eliminate below diagonal
	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 4; j++) {
			if (mat.matrix[j][i] == 0.0f) continue;
			float c = mat.matrix[j][i] / mat.matrix[i][i];
			for (int k = 0; k < 4; k++) {
				mat.matrix[j][k] -= mat.matrix[i][k] * c;
				inv.matrix[j][k] -= inv.matrix[i][k] * c;
			}
			mat.matrix[j][i] = 0.f;
		}
	}
	//    (3) set diagonal elements to zero
	for (int i = 0; i < 4; i++) {
		if (mat.matrix[i][i] == 0.0f) {
			fprintf(stderr, "Singular matrix, couldn't invert\n");
			return identity();
		};
		float k = 1 / (float)mat.matrix[i][i];
		for (int j = 0; j < 4; j++) {
			mat.matrix[i][j] *= k;
			inv.matrix[i][j] *= k;
		}
	}
	//    (4) eliminate above digonal
	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 4; j++) {
			float c = mat.matrix[i][j];
			for (int k = 0; k < 4; k++) {
				mat.matrix[i][k] -= mat.matrix[j][k] * c;
				inv.matrix[i][k] -= inv.matrix[j][k] * c;
			}
			mat.matrix[i][j] = 0.f;
		}
	}
	return inv;
}

void transform_vector(t_matrix transform, Vector3* vector) {
	if (vector == NULL) {
		fprintf(stderr, "NULL vector\n");
		return;
	}

	float x = vector->x;
	float y = vector->y;
	float z = vector->z;

	vector->x = transform.matrix[0][0] * x + transform.matrix[0][1] * y + transform.matrix[0][2] * z;
	vector->y = transform.matrix[1][0] * x + transform.matrix[1][1] * y + transform.matrix[1][2] * z;
	vector->z = transform.matrix[2][0] * x + transform.matrix[2][1] * y + transform.matrix[2][2] * z;
}

void transform_point(t_matrix transform, Vector3* vector) {
	if (vector == NULL) {
		fprintf(stderr, "NULL vector\n");
		return;
	}
	float x = vector->x;
	float y = vector->y;
	float z = vector->z;

	vector->x = transform.matrix[0][0] * x + transform.matrix[0][1] * y + transform.matrix[0][2] * z + transform.matrix[0][3];
	vector->y = transform.matrix[1][0] * x + transform.matrix[1][1] * y + transform.matrix[1][2] * z + transform.matrix[1][3];
	vector->z = transform.matrix[2][0] * x + transform.matrix[2][1] * y + transform.matrix[2][2] * z + transform.matrix[2][3];
}



