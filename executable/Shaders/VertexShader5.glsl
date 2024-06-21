#version 330 core
layout (location = 0) in vec3 input_position;
layout (location = 1) in vec2 input_texture_coordinates;

out vec2 texture_coordinates;

void main()
{
    texture_coordinates = input_texture_coordinates;
    gl_Position = vec4(input_position, 1.0);
}