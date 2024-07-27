#include "../includes/config.h"
#include <string.h>
#include "../includes/vector.h"
#include "../includes/transformations.h"
#include "../includes/object.h"
#include "../includes/color.h"

float degrees_to_radians(float angle) {
	return angle * ((float)M_PI / 180);
}

void Rotate_Object_X(Object3D* object, float angle_Deg) {
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, transformation failed\n");
		return;
	}
	t_matrix r = t_rotate_x(degrees_to_radians(angle_Deg));
	t_matrix inv = inverse(r);
	for (int i = 0; i < object->num_normals; i++) {
		transform_vector(inv, &object->normals[i]);
		object->normals[i] = normalize(object->normals[i]);
	}
	for (int i = 0; i < object->num_vertices; i++) {
		transform_vector(r, &object->vertices[i]);
	}
	printf("Object <%s> rotated on X\n", object->name);
}

void Rotate_Object_Y(Object3D* object, float angle_Deg) {
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, transformation failed\n");
		return;
	}
	t_matrix r = t_rotate_y(degrees_to_radians(angle_Deg));
	t_matrix inv = inverse(r);
	for (int i = 0; i < object->num_normals; i++) {
		transform_vector(inv, &object->normals[i]);
		object->normals[i] = normalize(object->normals[i]);
	}
	for (int i = 0; i < object->num_vertices; i++) {
		transform_vector(r, &object->vertices[i]);
	}
	printf("Object <%s> rotated on Y\n", object->name);
}

void Rotate_Object_Z(Object3D* object, float angle_Deg) {
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, transformation failed\n");
		return;
	}
	t_matrix r = t_rotate_z(degrees_to_radians(angle_Deg));
	t_matrix inv = inverse(r);
	for (int i = 0; i < object->num_normals; i++) {
		transform_vector(inv, &object->normals[i]);
		object->normals[i] = normalize(object->normals[i]);
	}
	for (int i = 0; i < object->num_vertices; i++) {
		transform_vector(r, &object->vertices[i]);
	}
	printf("Object <%s> rotated on Z\n", object->name);
}

void Quaternion_Rotate(Object3D* object, Vector3 axis, float angle_Deg) {
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, transformation failed\n");
		return;
	}
	Quaternion q = quaternion_from_axis_angle(axis, angle_Deg);
	t_matrix r = quaternion_to_matrix(q);
	t_matrix inv = inverse(r);
	for (int i = 0; i < object->num_normals; i++) {
		transform_vector(inv, &object->normals[i]);
		object->normals[i] = normalize(object->normals[i]);
	}
	for (int i = 0; i < object->num_vertices; i++) {
		transform_vector(r, &object->vertices[i]);
	}
	printf("Object <%s> rotated with quaternion\n", object->name);
}

void Rotate_Euler_ZYX(Object3D* object, float roll, float pitch, float yaw) {
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, transformation failed\n");
		return;
	}
	t_matrix r = euler_to_matrix((Euler_Angles) { roll, pitch, yaw });
	t_matrix inv = inverse(r);
	for (int i = 0; i < object->num_normals; i++) {
		transform_vector(inv, &object->normals[i]);
		object->normals[i] = normalize(object->normals[i]);
	}
	for (int i = 0; i < object->num_vertices; i++) {
		transform_vector(r, &object->vertices[i]);
	}
	printf("Object <%s> rotated with Euler ZYX\n", object->name);
}

void Object_Scale(Object3D* object, float factor) {
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, transformation failed\n");
		return;
	}
	t_matrix s = t_scale(factor);
	for (int i = 0; i < object->num_vertices;i++) {
		transform_vector(s, &object->vertices[i]);
	}
	printf("Object <%s> scaled\n", object->name);
}

void Object_Scale_XYZ(Object3D* object, float factor_x, float factor_y, float factor_z) {
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, transformation failed\n");
		return;
	}
	if (factor_x == 0.0f || factor_y == 0.0f || factor_z == 0.0f) {
		fprintf(stderr, "Invalid scaling: scale factor 0\n");
		return;
	}
	t_matrix s = t_scale_xyz(factor_x, factor_y, factor_z);
	t_matrix inv = inverse(s);
	for (int i = 0; i < object->num_normals; i++) {
		transform_vector(inv, &object->normals[i]);
		object->normals[i] = normalize(object->normals[i]);
	}
	for (int i = 0; i < object->num_vertices;i++) {
		transform_vector(s, &object->vertices[i]);
	}
	printf("Object <%s> scaled\n", object->name);
}

void Object_Translate_XYZ(Object3D* object, float tx, float ty, float tz) {
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, transformation failed\n");
		return;
	}
	t_matrix t = t_translate(tx, ty, tz);
	for (int i = 0; i < object->num_vertices;i++) {
		transform_point(t, &object->vertices[i]);
	}
	printf("Object <%s> translated\n", object->name);
}

int Export_OBJ(Object3D* object, char* filename) {
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, couldn't export\n");
		return -1;
	}
	const char* extension = strrchr(filename, '.');
	char* corrected_name;
	if (extension == NULL) {
		int len = strlen(filename);
		corrected_name = (char*)malloc(len + 5);
		strcpy(corrected_name, filename);
		strcpy(corrected_name + len, ".obj");
	}
	else if (strcmp(extension, ".obj") != 0) {
		fprintf(stderr, "Invalid extension, ensure .obj extension\n");
		return -1;
	}
	else {
		corrected_name = filename;
	}
	FILE* file = fopen(corrected_name, "w"); //updates original .obj if name is the same
	if (corrected_name != filename) {
		//free memory if it was allocated
		free(corrected_name);
	}
	if (file == NULL) {
		fprintf(stderr, "Couldn't export file\n");
		return -1;
	}
	if (object->name != NULL) {
		fprintf(file, "o %s\n", object->name);
	}
	if (object->mtllib != NULL) {
		fprintf(file, "mtllib %s\n", object->mtllib);
	}
	for (int i = 0; i < object->num_vertices; i++) {
		fprintf(file, "v %f %f %f\n", object->vertices[i].x, object->vertices[i].y, object->vertices[i].z);
	}
	for (int i = 0; i < object->num_normals; i++) {
		fprintf(file, "vn %f %f %f\n", object->normals[i].x, object->normals[i].y, object->normals[i].z);
	}
	for (int i = 0; i < object->num_texture_coord; i++) {
		fprintf(file, "vt %f %f\n", object->texture_coord[i].x, object->texture_coord[i].y);
	}
	fprintf(file, "s %d\n", object->smooth);
	if (object->usemtl != NULL) {
		fprintf(file, "usemtl %s\n", object->usemtl);
	}
	for (int i = 0; i < object->num_faces; i++) {
		fprintf(file, "f");
		for (int j = 0; j < 3; j++) {
			fprintf(file, " %d", object->faces[i].v_indices[j]);
			if (object->faces[i].vt_indices != NULL) {
				fprintf(file, "/%d", object->faces[i].vt_indices[j]);
			}
			if (object->faces[i].vn_indices != NULL) {
				if (object->faces[i].vt_indices != NULL) {
					fprintf(file, "/%d", object->faces[i].vn_indices[j]);
				}
				else {
					fprintf(file, "//%d", object->faces[i].vn_indices[j]);
				}
			}
		}
		fprintf(file, "\n");
	}
	printf("Object <%s> exported sucessfully: %s\n", object->name, filename);

	return 0;

}

Object3D* Read_Wavefront(char* filename) {

	const char* extension = strrchr(filename, '.');
	if (!extension || strcmp(extension, ".obj") != 0) {
		fprintf(stderr, "Invalid file format (wavefront .obj file required)\n");
		return NULL;
	}
	FILE* obj = fopen(filename, "r");
	if (obj == NULL) {
		fprintf(stderr, "Couldn't open .obj file\n");
		return NULL;
	}
	Object3D* newobject = (Object3D*)malloc(sizeof(Object3D));
	newobject->mtllib = NULL;
	newobject->usemtl = NULL;
	newobject->name = NULL;

	newobject->vertices = (Vector3*)malloc(MAX_VERTEX * sizeof(Vector3));
	newobject->normals = NULL;
	newobject->texture_coord = NULL;
	newobject->faces = (face*)malloc(MAX_FACES * sizeof(face));

	newobject->num_vertices = 0;
	newobject->num_normals = 0;
	newobject->num_texture_coord = 0;
	newobject->num_faces = 0;

	for (int i = 0; i < MAX_FACES; i++) {
		newobject->faces[i].v_indices = (int*)malloc(sizeof(int) * 3);
		newobject->faces[i].vt_indices = (int*)malloc(sizeof(int) * 3);
		newobject->faces[i].vn_indices = (int*)malloc(sizeof(int) * 3);
	}

	// Initialize material for local rendering and origin
	newobject->isGlass = 0;
	newobject->origin = vector_zero;
	newobject->color = red;

	char line[256]; // buffer for reading file
	int count_v = 0; // vertex count
	int count_vn = 0; // normals count
	int count_vt = 0; // texture coordinates count
	int count_f = 0; // faces count

	while (fgets(line, sizeof(line), obj) != NULL) {
		if (strncmp(line, "v ", 2) == 0) {
			sscanf(line + 2, "%f %f %f", &newobject->vertices[count_v].x, &newobject->vertices[count_v].y, &newobject->vertices[count_v].z);
			count_v++;
			if (count_v >= MAX_VERTEX) {
				fprintf(stderr, "Vertex limit reached (vertices): %d\n", MAX_VERTEX);
				Free_OBJ(newobject);
				return NULL;
			}
		}//===========================================================================================
		else if (strncmp(line, "vn ", 3) == 0) {
			if (newobject->normals == NULL) {
				newobject->normals = (Vector3*)malloc(MAX_VERTEX * sizeof(Vector3));
			}
			sscanf(line + 3, "%f %f %f", &newobject->normals[count_vn].x, &newobject->normals[count_vn].y, &newobject->normals[count_vn].z);
			count_vn++;
			if (count_vn >= MAX_VERTEX) {
				fprintf(stderr, "Vertex limit reached (normals): %d\n", MAX_VERTEX);
				Free_OBJ(newobject);
				return NULL;
			}
		}//===========================================================================================
		else if (strncmp(line, "vt ", 3) == 0) {
			if (newobject->texture_coord == NULL) {
				newobject->texture_coord = (Coordinate*)malloc(MAX_VERTEX * sizeof(Coordinate));
			}
			sscanf(line + 3, "%f %f", &newobject->texture_coord[count_vt].x, &newobject->texture_coord[count_vt].y);
			count_vt++;
			if (count_vt >= MAX_VERTEX) {
				fprintf(stderr, "Vertex limit reached (texture coordinates): %d\n", MAX_VERTEX);
				Free_OBJ(newobject);
				return NULL;
			}
		}//===========================================================================================
		else if (strncmp(line, "f ", 2) == 0) {
			int v_indices[MAX_FACE_SIZE_SUPPORTED];
			int vt_indices[MAX_FACE_SIZE_SUPPORTED];
			int vn_indices[MAX_FACE_SIZE_SUPPORTED];
			int num_vertices = 0;

			char* token = strtok(line + 2, " ");
			while (token != NULL) {
				if (num_vertices > MAX_FACE_SIZE_SUPPORTED - 1) {
					fprintf(stderr, "Invalid face: face size too large\n");
					Free_OBJ(newobject);
					return NULL;
				}
				if (count_vn > 0 && count_vt > 0) {
					sscanf(token, "%d/%d/%d", &v_indices[num_vertices], &vt_indices[num_vertices], &vn_indices[num_vertices]);
				}
				else if (count_vt > 0) {
					sscanf(token, "%d/%d", &v_indices[num_vertices], &vt_indices[num_vertices]);
				}
				else if (count_vn > 0) {
					sscanf(token, "%d//%d", &v_indices[num_vertices], &vn_indices[num_vertices]);
				}
				else {
					sscanf(token, "%d", &v_indices[num_vertices]);
				}
				token = strtok(NULL, " ");
				num_vertices++;
			}

			if (num_vertices < 3) {
				fprintf(stderr, "Invalid face skipped\n");
				continue;
			}

			// triangulate if needed
			for (int i = 1; i < num_vertices - 1; i++) {
				if (count_f >= MAX_FACES) {
					fprintf(stderr, "Face limit reached: %d\n", MAX_VERTEX);
					Free_OBJ(newobject);
					return NULL;
				}

				newobject->faces[count_f].v_indices[0] = v_indices[0];
				newobject->faces[count_f].v_indices[1] = v_indices[i];
				newobject->faces[count_f].v_indices[2] = v_indices[i + 1];

				if (count_vt > 0) {
					newobject->faces[count_f].vt_indices[0] = vt_indices[0];
					newobject->faces[count_f].vt_indices[1] = vt_indices[i];
					newobject->faces[count_f].vt_indices[2] = vt_indices[i + 1];
				}
				else {
					free(newobject->faces[count_f].vt_indices);
					newobject->faces[count_f].vt_indices = NULL;
				}

				if (count_vn > 0) {
					newobject->faces[count_f].vn_indices[0] = vn_indices[0];
					newobject->faces[count_f].vn_indices[1] = vn_indices[i];
					newobject->faces[count_f].vn_indices[2] = vn_indices[i + 1];
				}
				else {
					free(newobject->faces[count_f].vt_indices);
					newobject->faces[count_f].vt_indices = NULL;
				}

				newobject->faces[count_f].num_vertices = 3;
				count_f++;
			}
		}//===========================================================================================
		else if (strncmp(line, "o ", 2) == 0) {
			size_t len = strlen(line + 2);
			newobject->name = (char*)malloc((len) * sizeof(char));
			if (newobject->name != NULL) {
				strncpy(newobject->name, line + 2, len);
				newobject->name[len - 1] = '\0';
			}
		}//===========================================================================================
		else if (strncmp(line, "s ", 2) == 0) {
			sscanf(line + 2, "%d", &newobject->smooth);
		}//===========================================================================================
		else if (strncmp(line, "mtllib ", 6) == 0) {
			size_t len = strlen(line + 6);
			newobject->mtllib = (char*)malloc((len) * sizeof(char));
			if (newobject->mtllib != NULL) {
				strncpy(newobject->mtllib, line + 6, len);
				newobject->mtllib[len - 1] = '\0';
			}
		}//===========================================================================================
		else if (strncmp(line, "usemtl ", 6) == 0) {
			size_t len = strlen(line + 6);
			newobject->usemtl = (char*)malloc((len) * sizeof(char));
			if (newobject->usemtl != NULL) {
				strncpy(newobject->usemtl, line + 6, len);
				newobject->usemtl[len - 1] = '\0';
			}
		}
	}
	if (count_v == 0) {
		fprintf(stderr, "No vertices found, invalid object\n");
		Free_OBJ(newobject);
		return NULL;
	}
	newobject->num_vertices = count_v;
	newobject->num_normals = count_vn;
	newobject->num_texture_coord = count_vt;
	newobject->num_faces = count_f;

	printf("Mesh parsed sucessfully: %s\n", filename);
	fclose(obj);
	return newobject;
}

void Free_OBJ(Object3D* object) {
	//assuming memory was already allocated for faces and vertices
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, couldn't free memory\n");
		return;
	}
	for (int i = 0; i < MAX_FACES; i++) {
		free(object->faces[i].v_indices);
		free(object->faces[i].vn_indices);
		free(object->faces[i].vt_indices);
	}
	if (object->normals != NULL) free(object->normals);
	if (object->texture_coord != NULL) free(object->texture_coord);
	if (object->usemtl != NULL) free(object->usemtl);
	if (object->mtllib != NULL) free(object->mtllib);
	if (object->name != NULL) free(object->name);

	free(object->vertices);
	free(object->faces);
	free(object);
	fprintf(stderr, "Object memory set free\n");
}

void Set_Material_OBJ(Object3D* object, int isGlass, Color color) {
	if (object == NULL) {
		fprintf(stderr, "Object was NULL pointer, couldn't set material\n");
		return;
	}
	if (isGlass != 0 && isGlass != 1) {
		fprintf(stderr, "Invalid input: glass can be either 0 or 1\n");
		return;
	}
	object->color = color;
	object->isGlass = isGlass;
	printf("Material set for <%s>\n", object->name);
}

// Functions below are for debugging

void print_faces_test(Object3D* object) {
	for (int i = 0; i < object->num_faces; i++) {
		face* f = &object->faces[i];
		printf("Face %d:\n", i + 1);

		for (int j = 0; j < f->num_vertices; j++) {
			int vertexIndex = f->v_indices[j] - 1;
			Vector3 vertex = object->vertices[vertexIndex];
			printf("\tVertex %d: (%f, %f, %f)\n", f->v_indices[j], vertex.x, vertex.y, vertex.z);
		}
	}
}
void console_print(Object3D* object) {

	if (object->name != NULL) {
		printf("o %s\n", object->name);
	}
	if (object->mtllib != NULL) {
		printf("mtllib %s\n", object->mtllib);
	}
	for (int i = 0; i < object->num_vertices; i++) {
		printf("v %f %f %f\n", object->vertices[i].x, object->vertices[i].y, object->vertices[i].z);
	}

	for (int i = 0; i < object->num_normals; i++) {
		printf("vn %f %f %f\n", object->normals[i].x, object->normals[i].y, object->normals[i].z);
	}
	for (int i = 0; i < object->num_texture_coord; i++) {
		printf("vt %f %f\n", object->texture_coord[i].x, object->texture_coord[i].y);
	}
	printf("s %d\n", object->smooth);
	if (object->usemtl != NULL) {
		printf("usemtl %s\n", object->usemtl);
	}
	for (int i = 0; i < object->num_faces; i++) {
		printf("f");
		for (int j = 0; j < 3; j++) {
			printf(" %d", object->faces[i].v_indices[j]);
			if (object->faces[i].vt_indices != NULL) {
				printf("/%d", object->faces[i].vt_indices[j]);
			}
			if (object->faces[i].vn_indices != NULL) {
				if (object->faces[i].vt_indices != NULL) {
					printf("/%d", object->faces[i].vn_indices[j]);
				}
				else {
					printf("//%d", object->faces[i].vn_indices[j]);
				}
			}
		}
		printf("\n");
	}
	printf("\n");

}
static void countElements(FILE* file, int* vertices, int* normals, int* texture_coord, int* faces) {
	char buffer[100];
	while (fgets(buffer, 100, file) != NULL) {
		if (strncmp(buffer, "v ", 2) == 0) {
			(*vertices)++;
		}
		else if (strncmp(buffer, "vn", 2) == 0) {
			(*normals)++;
		}
		else if (strncmp(buffer, "vt", 2) == 0) {
			(*texture_coord)++;
		}
		else if (strncmp(buffer, "f ", 2) == 0) {
			(*faces)++;
		}

		memset(buffer, 0, sizeof(buffer));
	}
	printf("%d vertices\n", *vertices);
	printf("%d normals\n", *normals);
	printf("%d texture_coord\n", *texture_coord);
	printf("%d faces\n\n", *faces);
	fseek(file, 0, SEEK_SET);
}

//for debugging
//int f = 2;
//Vector3 normal1 = normalize((Vector3) { cube->normals[cube->faces[f].vn_indices[0] - 1].x, cube->normals[cube->faces[f].vn_indices[0] - 1].y, cube->normals[cube->faces[f].vn_indices[0] - 1].z });
//printf("File normal:%f %f %f\n", normal1.x, normal1.y, normal1.z);
//Vector3 normal2 = normalize(cross(vector_sub(cube->vertices[cube->faces[f].v_indices[1] - 1], cube->vertices[cube->faces[f].v_indices[0] - 1]), vector_sub(cube->vertices[cube->faces[f].v_indices[2] - 1], cube->vertices[cube->faces[f].v_indices[0] - 1])));
//printf("Calculated normal: %f %f %f\n", normal2.x, normal2.y, normal2.z);