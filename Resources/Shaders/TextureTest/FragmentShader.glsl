#version 330 core
out vec4 FragColor;

uniform sampler2D image_texture;

in VS_OUT
{
  vec2 texture_coordinates;
} fs_in;

void main()
{

  FragColor = texture(image_texture, fs_in.texture_coordinates);
}