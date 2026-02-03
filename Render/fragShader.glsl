#version 430

in vec3 vColor;

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 p_matrix;

void main(void)
{ color = vec4(vColor, 1.0); }