#version 330 core
out vec4 FragColor;

in vec2 texture_coordinates;

uniform sampler2D hdr_buffer;
uniform bool hdr;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdr_color = texture(hdr_buffer, texture_coordinates).rgb;
    if(hdr)
    {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
        vec3 result = vec3(1.0) - exp(-hdr_color * exposure);
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdr_color, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}