#version 330 core
layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;

out VS_OUT
{
  vec3 fragment_position;
  vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.fragment_position = vec3(model * vec4(input_position, 1.0));
    vs_out.normal = mat3(transpose(inverse(model))) * input_normal;  
    
    gl_Position = projection * view * vec4(vs_out.fragment_position, 1.0);
}