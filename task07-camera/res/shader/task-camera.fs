#version 330 core
out vec4 FragColor;

in vec3 positionColor;

void main()
{
	FragColor = vec4(positionColor,1.0);
}