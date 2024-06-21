#version 330 core
layout (location = 0) in vec3 input_position;

uniform mat4 light_space_matrix;
uniform mat4 model;

void main()
{
    gl_Position = light_space_matrix * model * vec4(input_position, 1.0);
}