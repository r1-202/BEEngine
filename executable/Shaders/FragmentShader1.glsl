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

  sampler2D diffuse_texture;
  sampler2D specular_texture;
  sampler2D normal_map;
};

struct Directional_Light {
  vec3 direction;

  vec3 ambient_color;
  vec3 diffuse_color;
  vec3 specular_color;
};

struct Point_Light {
    vec3 position;

    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

struct Spotlight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outer_cutoff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;       
};

#define NR_POINT_LIGHTS 4
in VS_OUT
{
  vec3 fragment_position;
  vec3 normal;
  vec2 texture_coordinates;
} fs_in;

uniform vec3 view_position;
uniform Directional_Light directional_light;
uniform Point_Light point_lights[NR_POINT_LIGHTS];
uniform Spotlight spotlight;
uniform Material material;

// function prototypes
vec3 calculateDirectionalLight(Directional_Light light, vec3 normal, vec3 view_direction);
vec3 calculatePointLight(Point_Light light, vec3 normal, vec3 fragment_position, vec3 view_direction);
vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 fragment_position, vec3 view_direction);

void main()
{    
    // properties
    vec3 normal = normalize(fs_in.normal);
    vec3 view_direction = normalize(view_position - fs_in.fragment_position);
    
    // phase 1: directional lighting
    vec3 result = vec3(0.,0.,0.);
    //result+=calculateDirectionalLight(directional_light, normal, view_direction);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += calculatePointLight(point_lights[i], normal, fs_in.fragment_position, view_direction);    
    // phase 3: spot light
    //result += calculateSpotlight(spotlight, normal, fs_in.fragment_position, view_direction);    
    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 calculateDirectionalLight(Directional_Light light, vec3 normal, vec3 view_direction)
{
    vec3 light_direction = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.specular_exponent);
    // combine results
    vec3 ambient = light.ambient_color * vec3(texture(material.diffuse_texture, fs_in.texture_coordinates));
    vec3 diffuse = light.diffuse_color * diff * vec3(texture(material.diffuse_texture, fs_in.texture_coordinates));
    vec3 specular = light.specular_color * spec * vec3(texture(material.specular_texture, fs_in.texture_coordinates));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 calculatePointLight(Point_Light light, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - fs_in.fragment_position);
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.specular_exponent);
    // attenuation
    float distance = length(light.position - fs_in.fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient_color * vec3(texture(material.diffuse_texture, fs_in.texture_coordinates));
    vec3 diffuse = light.diffuse_color * diff * vec3(texture(material.diffuse_texture, fs_in.texture_coordinates));
    vec3 specular = light.specular_color * spec * vec3(texture(material.specular_texture, fs_in.texture_coordinates));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - fs_in.fragment_position);
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    // specular shading
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.specular_exponent);
    // attenuation
    float distance = length(light.position - fs_in.fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(light_direction, normalize(-light.direction)); 
    float epsilon = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient_color * vec3(texture(material.diffuse_texture, fs_in.texture_coordinates));
    vec3 diffuse = light.diffuse_color * diff * vec3(texture(material.diffuse_texture, fs_in.texture_coordinates));
    vec3 specular = light.specular_color * spec * vec3(texture(material.specular_texture, fs_in.texture_coordinates));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}