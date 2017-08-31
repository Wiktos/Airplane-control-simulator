#version 330 core
out vec4 fragment_color;

in vec2 texCoords;

uniform sampler2D texture_diffuse1;

void main()
{
	fragment_color = texture(texture_diffuse1, texCoords);
}