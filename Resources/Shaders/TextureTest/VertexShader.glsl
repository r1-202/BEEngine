#version 330 core
layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 2) in vec2 input_texture_coordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
  vec2 texture_coordinates;
} vs_out;

void main()
{
  gl_Position = projection*view*model*vec4(input_position, 1.);
  vs_out.texture_coordinates = input_texture_coordinates;
}