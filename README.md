# Simple ray-tracer in C
<p align="center">
    <img src="/Rendering_&_transforms_in_C/renders/monkey.png" alt="Rendered image example" width="500"/>
</p>

## What does this program do
You can:
- read/export wavefront files (files used for 3d objects with .obj extension)
- do basic transformations on objects such as rotatoin, scale and translation
- render object with simple local renderer

## How to run
In visual studio you can do this:
1) Open Visual studio
2) Go to **Clone repository**
3) Set up URL and path
4) Clone
5) Open visual studio again and go to **Open a project or solution**
6) Click on **solution view** or double click .sln file
7) Set up main function and run the program

Here it is explained very clearly:

https://learn.microsoft.com/en-us/visualstudio/get-started/tutorial-open-project-from-repo?view=vs-2022
https://github.com/MicrosoftDocs/visualstudio-docs/blob/main/docs/get-started/tutorial-open-project-from-repo.md

## How to use
Everything is controlled in `main()` function, there is no console or windows interface, this is only a simple rendering software. You can see in the example how thing are set up:
1) Adjust settings in config.h accoring to your preferences
2) Read object, set material, transform it and export it if you want
3) Set up objects array and lights array (currently rendering supports a single object and light)
4) Set up camera (point look_at which is where camera is looking, eye which is camera location, fov and aspect ratio ideally set to `(float)WIDTH/HEIGHT`
5) Render the scene (Color* is image which is array or flat memory matrix of colors)
6) Export image with `stbi_image_write()` which is available in stb_image_write.h library from Sean Barrett

More info about functions is available in .h or .c files

**REMEMBER TO EDIT CONFIG.H**

### Features:
- faces are triangulated while reading wavefront (fan triangulation)
- Exported object will maintain triangulated faces
- Moller Trumbore algorighm is used for ray triangle intersection
- Gauss-Jordan algorithm is used for inverse matrix
- You can follow render progress on console

### Not supported:
- preserving group information in .obj files
- reading .mtl files
- rendering multiple objects/lights and shadows
- n-gon faces and smooth shading
- undo transforms

### Improvement to do:
- Render multiple objects/lights
- Better shading with isGlass information (reflection and refraction)
- Origin of object and fixes for scaling
- OpenGL or windows.h for GUI
