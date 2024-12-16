#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

layout (std140, column_major) uniform Matrices 
{
	mat4 u_viewMatrix;
	mat4 u_projectionMatrix;
};

uniform float pointSize;

out vec4 oColor;

void main() {
	vec4 worldPos = u_projectionMatrix * u_viewMatrix * vec4(position, 1.0);
	mat4 m;
	gl_Position = u_projectionMatrix * u_viewMatrix * vec4(position, 1.0);
	gl_PointSize = pointSize;
	oColor = color;
}