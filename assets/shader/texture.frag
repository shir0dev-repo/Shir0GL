#version 460 core

in vec2 tcoord;
in vec3 normal;

uniform sampler2D u_texture;

out vec4 FragColor;

void main() {
	FragColor = texture(u_texture, tcoord);
}