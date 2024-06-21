#version 330 core
out vec4 FragColor;

in VS_OUT
{
  vec3 fragment_position;
  vec2 texture_coordinates;
  vec3 light_position_tangent_space;
  vec3 view_position_tangent_space;
  vec3 fragment_position_tangent_space;
} fs_in;

struct Material
{
  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
  vec3 emission_color;

  float alpha;
  float specular_exponent;
  float index_of_refraction;

  sampler2D diffuse_texture;
  sampler2D specular_texture;
  sampler2D normal_map;
};

uniform Material material;

uniform vec3 light_position;
uniform vec3 view_position;

void main()
{           
     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(material.normal_map, fs_in.texture_coordinates).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(material.diffuse_texture, fs_in.texture_coordinates).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 light_direction = normalize(fs_in.light_position_tangent_space - fs_in.fragment_position_tangent_space);
    float diff = max(dot(light_direction, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 view_direction = normalize(fs_in.view_position_tangent_space - fs_in.fragment_position_tangent_space);
    vec3 reflect_direction = reflect(-light_direction, normal);
    vec3 halfway_direction = normalize(light_direction + view_direction);  
    float spec = pow(max(dot(normal, halfway_direction), 0.0), material.specular_exponent);

    vec3 specular = spec * texture(material.specular_texture,fs_in.texture_coordinates).rgb;
    vec3 result = ambient + diffuse + specular;
    result = pow(result, vec3(1.0/2.2));
    FragColor = vec4(result, 1.0);
}