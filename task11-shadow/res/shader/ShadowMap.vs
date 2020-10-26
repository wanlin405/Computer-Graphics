#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 lightPV;

void main()
{
	gl_Position = lightPV * model * vec4(aPos, 1.0);
}