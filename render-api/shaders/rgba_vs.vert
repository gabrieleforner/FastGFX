#version 450

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec4 vertColor;

layout(location = 1) out vec4 outVertColor;

void main()
{
	outVertColor = vertColor;
	gl_Position = vec4(vertPos, 1.0);
}