#version 330 core
layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 2) in vec2 input_texture_coordinates;
layout (location = 3) in vec3 input_tangent;
layout (location = 4) in vec3 input_bitangent;

out VS_OUT
{
  vec3 fragment_position;
  vec2 texture_coordinates;
  vec3 light_position_tangent_space;
  vec3 view_position_tangent_space;
  vec3 fragment_position_tangent_space;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 light_position;
uniform vec3 view_position;

void main()
{
    vs_out.fragment_position = vec3(model * vec4(input_position, 1.0));   
    vs_out.texture_coordinates = input_texture_coordinates;
    
    mat3 normal_matrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normal_matrix * input_tangent);
    vec3 N = normalize(normal_matrix * input_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out.light_position_tangent_space = TBN * light_position;
    vs_out.view_position_tangent_space  = TBN * view_position;
    vs_out.fragment_position_tangent_space  = TBN * vs_out.fragment_position;
        
    gl_Position = projection * view * model * vec4(input_position, 1.0);
}