#version 450

layout(location=1) in vec4 color;		// Took by the VS
layout(location=0) out vec4 outColor;

void main()
{
	outColor = color;
}