#version 330 core
layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 2) in vec2 input_texture_coordinates;

out VS_OUT
{
  vec3 fragment_position;
  vec4 fragment_position_light;
  vec3 normal;
  vec2 texture_coordinates;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_space_matrix;//a projection*view matrix

void main()
{
    vs_out.fragment_position = vec3(model * vec4(input_position, 1.0));
    vs_out.normal = mat3(transpose(inverse(model))) * input_normal;  
    vs_out.texture_coordinates = input_texture_coordinates;
    vs_out.fragment_position_light = light_space_matrix*vec4(vs_out.fragment_position,1.0);

    gl_Position = projection * view * vec4(vs_out.fragment_position, 1.0);
}