#version 440 core

in vec3 voxelColor;

out vec4 Color;

void main() {
	Color = vec4(voxelColor, 1.0);
}