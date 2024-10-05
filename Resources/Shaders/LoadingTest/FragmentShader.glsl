#version 330 core
out vec4 FragColor;

struct Material
{
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  vec3 emission_color;

  float alpha;
  float specular_exponent;
  float index_of_refraction;

  sampler2D diffuse_map;
  sampler2D specular_map;
  sampler2D normal_map;
};


in VS_OUT
{
  vec3 fragment_position;
  vec3 normal;
  vec2 texture_coordinates;
} fs_in;

uniform Material material;

void main()
{        
    FragColor = texture(material.diffuse_map,fs_in.texture_coordinates);
}
