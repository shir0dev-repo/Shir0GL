#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTcoord;
layout (location = 2) in vec3 aNormal;

layout (std140, column_major) uniform Matrices 
{
	mat4 u_viewMatrix;
	mat4 u_projectionMatrix;
};

uniform mat4 u_transformationMatrix;

out vec2 tcoord;
out vec3 normal;

void main() {
	vec4 worldPosition = u_transformationMatrix * vec4(aPosition.xyz, 1.0);
	gl_Position = u_projectionMatrix * u_viewMatrix * worldPosition;
	
	normal = aNormal;
	tcoord = aTcoord;
}