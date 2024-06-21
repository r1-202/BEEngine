#version 330 core
out vec4 FragColor;


struct Point_Light {
    vec3 position;

    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

in VS_OUT
{
  vec3 fragment_position;
  vec3 normal;
} fs_in;

uniform vec3 view_position;
uniform Point_Light point_light;

uniform vec3 color;

// function prototypes
vec3 calculatePointLight(Point_Light light, vec3 normal, vec3 fragment_position, vec3 view_direction);

void main()
{    
    // properties
    vec3 normal = normalize(fs_in.normal);
    vec3 view_direction = normalize(view_position - fs_in.fragment_position);
    
    vec3 result = vec3(0.,0.,0.);
    result += calculatePointLight(point_light, normal, fs_in.fragment_position, view_direction);    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 calculatePointLight(Point_Light light, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - fs_in.fragment_position);
    // diffuse shading
    float diff = max(dot(normal, light_direction), 0.0);
    // specular shading
    vec3 halfway_direction = normalize(light_direction+view_direction);
    float spec = pow(max(dot(normal, halfway_direction), 0.0), 264.0);
    // attenuation
    float distance = length(light.position - fs_in.fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient_color * color;
    vec3 diffuse = light.diffuse_color * diff * color;
    vec3 specular = light.specular_color * spec;
    //ambient *= attenuation;
    //diffuse *= attenuation;
    //specular *= attenuation;
    return (ambient + diffuse + specular);
}
