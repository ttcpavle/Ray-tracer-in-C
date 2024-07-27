#ifndef OBJECT_H
#define OBJECT_H

#include "vector.h"
#include "color.h"
#include "transformations.h"

typedef struct face {
	int* v_indices;
	int* vt_indices;
	int* vn_indices;
	int num_vertices;
} face;

/* =======================================================================
 -.mtl reading is not implemented here but exported file has original information about material (mtllib, usemtl)
 -renderer in this program uses simple shading so additinal properties are included only for rendering in here (they are not exported):
 =======================================================================*/
typedef struct Object3D {
	char* name;
	char* mtllib;
	Vector3* vertices;
	Coordinate* texture_coord;
	Vector3* normals;
	face* faces;
	int smooth;
	char* usemtl;
	int num_vertices;
	int num_texture_coord;
	int num_normals;
	int num_faces;
	//FOR LOCAL RENDERING
	int isGlass;
	Color color;
	Vector3 origin;
}Object3D;

float degrees_to_radians(float angle);

/* =======================================================================
 FUNCTION: Read_Wavefront
 Reads wavefront file format (.obj)
 File is searched within objects/ in project directory
 Properties:
 -reads .obj with given name
 -converts n-gon faces to triangles using fan triangulation algorithm (it doesn't work correctly on non convex faces)
 -you can manually adjust maximum vertices, maximum faces and max n-gon size for triangulation in config.h
 TO IMPLEMENT: group information
 =======================================================================*/
Object3D* Read_Wavefront(char* filename);

/* =======================================================================
 FUNCTION: Export_OBJ
 Export object as wavefront (updates original if file with given name exists)
 Default path is object/ in project directory
 supported filenames:
 -"name"
 -"name.obj"
=======================================================================*/
int Export_OBJ(Object3D* object, char* filename);

// Matrix rotations, quaternion rotation, ZYX euler rotation
void Rotate_Object_X(Object3D* object, float angle_Deg);
void Rotate_Object_Y(Object3D* object, float angle_Deg);
void Rotate_Object_Z(Object3D* object, float angle_Deg);
void Quaternion_Rotate(Object3D* object, Vector3 axis, float angle_Deg);
void Rotate_Euler_ZYX(Object3D* object, float roll, float pitch, float yaw);

// Object scaling
void Object_Scale(Object3D* object, float factor);
void Object_Scale_XYZ(Object3D* object, float factor_x, float factor_y, float factor_z);

// Translate object (origins will not change when importing in 3d softwares like blender)
void Object_Translate_XYZ(Object3D* object, float translate_x, float translate_y, float translate_z);

// Free memory
void Free_OBJ(Object3D* object);

// Set object color and glass properties
void Set_Material_OBJ(Object3D* object, int glass, Color color);

// Debugging help functions
void print_faces_test(Object3D* object);
void console_print(Object3D* object);

#endif