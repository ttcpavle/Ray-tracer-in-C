#include "../includes/config.h"
#include "../includes/object.h"
#include "../includes/transformations.h"
#include "../includes/render.h"
#include "../includes/stb_image_write.h"

/*=======================================================================
Here in main function you can modify object, export it and render it with local renderer
You can also run the example below
=======================================================================*/

int main() {
	// set up object
	Object3D* object = Read_Wavefront("objects/monkey.obj");
	Set_Material_OBJ(object, 0, lime);
	Object_Scale(object, 2);
	Export_OBJ(object, "objects/updated.obj");

	// set up scene
	Object3D* objects[10];
	objects[0] = object;
	Light l = { (Vector3) { -1.5f , 1.9f, 4.0f }, 14 };
	Light lights[10];
	lights[0] = l;
	Camera cam;
	Set_Camera(&cam, vector_zero, world_up, (Vector3) { 6.0f, 3.0f, 6.0f }, 45.0f, (float)WIDTH / HEIGHT);

	// render
	Color* image = Render(objects, lights, 1, 1, &cam);
	stbi_write_png("renders/render.png", WIDTH, HEIGHT, 3, image, WIDTH * sizeof(Color));

	return 0;
}