# CPURayTracer

## About
A CPU ray tracer built entirely from scratch in C++.

### Features
- Ambient, diffuse and specular shading
- Directional lights and attenuated point lights
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
Build the raytracer using the given Makefile via
>> make

Run using

>> ./raytracer scenefile [outputfile] [softshadows] [dof]

- **scenefile** - path to input file containing scene description
- **outputfile** - name for final output image file (optional)
- **softshadows** - soft shadow toggle, 0 = off, 1 = on (optional)
- **dof** - depth of field toggle, 0 = off, 1 = on (optional)

Note that it may take several seconds for the program to finish rendering the scene.

### Example
To render the demo scene included in this repo, navigate to this directory in your terminal of choice and—if you have yet to do so—build the project with
>> make

and then run the ray tracer on the demo scene:
>> ./raytracer scenes/demo-scene.txt

The output image will appear in a file located in the `scenes` folder named     `demo-scene.ppm`.
