#version 430
layout (location=0) in vec3 position;

uniform mat4 mv_matrix;
uniform mat4 p_matrix;
uniform float rotationCounter;

out vec3 vColor;

mat4 buildRotateY(float rad);

void main(void)
{ 
  mat4 localRotY = buildRotateY(rotationCounter);
  gl_Position = p_matrix * mv_matrix * localRotY * vec4(position, 1.0);
  vColor = position;
}

/* explore how I can place bounds on the incoming
   frequencies, scalar multiplication here only
   effects the amplitude of the lightwave not
   the frequency itself.
*/


mat4 buildRotateY(float rad)
{
	mat4 y_rot = mat4(cos(rad), 0.0, sin(rad), 0.0,
					  0.0, 1.0, 0.0, 0.0,
					  -sin(rad), 0.0, cos(rad), 0.0,
					  0.0, 0.0, 0.0, 1.0);
	return y_rot;
};