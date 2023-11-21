# CPURayTracer
<p align="center">
  <img alt="Depth of Field" src="https://github.com/matthiasbroske/CPURayTracer/assets/82914350/ece311fc-5531-4752-83a6-f2411e624faa" width="45%">
&nbsp; &nbsp; &nbsp; &nbsp;
  <img alt="Stanford Bunny" src="https://github.com/matthiasbroske/CPURayTracer/assets/82914350/2925d566-f921-4536-aa0c-ebdffa591e3b" width="45%">
</p>

## About
A CPU ray tracer built entirely from scratch in C++.

### Features
- Ambient, diffuse and specular shading
- Directional lights
- Attenuated point lights
- Depthcueing
- Hard shadows
- Soft shadows
- Reflection
- Transparency
- Refraction
- Depth of field
- Texture mapping
- BVH acceleration

## Usage
Build the ray tracer using the given Makefile with

```shell
make
```

and run using

```shell
./raytracer scenefile [outputfile] [softshadows] [dof]
```

- **scenefile** - path to input file containing scene description
- **outputfile** - name for final output image file (optional)
- **softshadows** - soft shadow toggle, 0 = off, 1 = on (optional)
- **dof** - depth of field toggle, 0 = off, 1 = on (optional)

Note that it may take several seconds for the ray tracer to complete rendering the scene.

### Example
To render the demo scene included in this repo, navigate to this directory in your terminal of choice and — if you have yet to do so — build the project with
```shell
make
```

and then run the ray tracer on the demo scene:
```shell
./raytracer scenes/demo-scene.txt
```

The output image will appear in a file named `demo-scene.ppm` located in the `scenes` folder.

<p align="center">
  <img alt="Demo Scene" src="https://github.com/matthiasbroske/CPURayTracer/assets/82914350/b3146f95-62f9-447a-9287-55b6f432b5bc">
</p>

## Scene Description Files
Scenes are defined in simple text files which contain information about the camera, lighting, materials, and the geometry of objects in the scene. For an example of a scene description file, see [`demo-scene.txt`](scenes/demo-scene.txt).

The syntax for scene description files is defined as follows, where entries in bold are keywords and entries in italics are variables:

### Camera
**eye** *eye<sub>x</sub>* *eye<sub>y</sub>* *eye<sub>z</sub>* (camera position)

**viewdir** *vdir<sub>x</sub>* *vdir<sub>y</sub>* *vdir<sub>z</sub>* (camera view direction)

**updir** *up<sub>x</sub>* *up<sub>y</sub>* *up<sub>z</sub>* (camera up direction)

**vfov** *vfov* (vertical field of view, in degrees)

**imsize** *width* *height* (size of the output image in pixel units)

**bkgcolor** *r* *g* *b* (background color in terms of r, g, b components defined on a scale from 0-1)

**viewdist** *d* (depth of field focus distance)

### Lighting
**light** *x* *y* *z* *w* *r* *g* *b* (*(x, y, z, 1)* = point light position, *(x, y, z, 0)* = directional light direction, with rgb light color)

**attlight** *x* *y* *z* *w* *r* *g* *b* *c<sub>1</sub>* *c<sub>2</sub>* *c<sub>3</sub>* (attenuated point light)

**depthcueing** *dc<sub>r</sub>* *dc<sub>g</sub>* *dc<sub>b</sub>* *a<sub>max</sub>* *a<sub>min</sub>* *dist<sub>max</sub>* *dist<sub>min</sub>* (depthcueing)

### Materials
**mtlcolor** *Od<sub>r</sub>* *Od<sub>g</sub>* *Od<sub>b</sub>* *Os<sub>r</sub>* *Os<sub>g</sub>* *Os<sub>b</sub>* *ka* *kd* *ks* *n* *α* *η* (material properties, treated as a state variable such that all subsequently-defined objects use the immediately-preceding material properties)

### Textures
**texture** ppm (path to ppm texture file, treated as a state variable in the same way as materials)

### Primitives
**sphere** *cx* *cy* *cz* *r*  (sphere defined by its center point and radius)

### Triangles
#### list of vertex positions
**v** *x<sub>1</sub>* *y<sub>1</sub>* *z<sub>1</sub>*\
**v** *x<sub>2</sub>* *y<sub>2</sub>* *z<sub>2</sub>*\
**v** *x<sub>3</sub>* *y<sub>3</sub>* *z<sub>3</sub>*\
**v** *x<sub>4</sub>* *y<sub>4</sub>* *z<sub>4</sub>*\
. . .
#### list of vertex normals
**vn** *nx<sub>1</sub>* *ny<sub>1</sub>* *nz<sub>1</sub>*\
**vn** *nx<sub>2</sub>* *ny<sub>2</sub>* *nz<sub>2</sub>*\
**vn** *nx<sub>3</sub>* *ny<sub>3</sub>* *nz<sub>3</sub>*\
**vn** *nx<sub>4</sub>* *ny<sub>4</sub>* *nz<sub>4</sub>*\
. . .
#### list of texture coordinates
**vt** *u<sub>1</sub>* *v<sub>1</sub>*\
**vt** *u<sub>2</sub>* *v<sub>2</sub>*\
**vt** *u<sub>3</sub>* *v<sub>3</sub>*\
**vt** *u<sub>4</sub>* *v<sub>4</sub>*\
. . .
#### list of triangle definitions, consisting of appropriate indices into the vertex arrays, starting at 1
**f** *v<sub>1</sub>/vt<sub>1</sub>/vn<sub>1</sub>* *v<sub>2</sub>/vt<sub>2</sub>/vn<sub>2</sub>* *v<sub>3</sub>/vt<sub>3</sub>/vn<sub>3</sub>*\
**f** *v<sub>1</sub>/vt<sub>1</sub>/vn<sub>1</sub>* *v<sub>2</sub>/vt<sub>2</sub>/vn<sub>2</sub>* *v<sub>4</sub>/vt<sub>4</sub>/vn<sub>4</sub>*\
. . .

Note that triangles are defined more-or-less according to the .obj file format, such that most .obj files should provide geometry combatible with this renderer.
