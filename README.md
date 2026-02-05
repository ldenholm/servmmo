### Checkout branches covering the journey so far

[Main](https://github.com/ldenholm/servmmo/tree/master)
Includes path tracing, static collisions (using olcPixelGameEngine),
AABBs and quad trees.

[Rendering cubes](https://github.com/ldenholm/servmmo/tree/3d_cube)
Switches to OpenGL using GLSL, GLFW, GLM. Explores rendering primitives,
implementing rotation and translation matrices, model view and projection
matrices. This branch forms the basic programmatic structure for the rendering 
engine I aim to expand.

[Multiple Objects Single Scene](https://github.com/ldenholm/servmmo/tree/cube_pyramid)
Using gradients in fragment shader. Drawing multiple objects in a single window.
Experimenting with animation mostly rotation along a single axis.

[Miniature Approximate Solar System](https://github.com/ldenholm/servmmo/tree/pyramid_system)
Creating a mini solar system using the cubes and pyramid from earlier work. Includes
z-axis zoom and matrix stacks for chaining matrix operations.

### Adding acceleration with RMB and drag, only static collisions

![Velocity_Drag_](Balls/velocity_drag.gif)
