#version 460 core

#define MAX_DIR_LIGHTS 1
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 1

struct DirectionalLight {
	vec4 direction;		//16
	vec4 diffuse;		//16
	vec4 specular;		//16
	bool isActive;		// 4
						//64B
};

struct PointLight {
	vec4 position;		//16
	vec4 diffuse;		//16
	vec4 specular;		//16
	
	bool isActive;		// 4

	vec4 attenuation;	//16
						//80B
};

struct SpotLight {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	
	bool isActive;

	vec4 attenuation;
	
	float innerAngle;
	float outerAngle;
	
	vec4 direction;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (std140, column_major) uniform Matrices 
{
	mat4 u_viewMatrix;
	mat4 u_projectionMatrix;
};

layout (std140) uniform Lights {
	DirectionalLight sunlight;
	PointLight pointLights[MAX_POINT_LIGHTS];
	SpotLight spotLights[MAX_SPOT_LIGHTS];
};

uniform mat4 u_transformationMatrix;

out v2f {
	vec3 worldPos;
	vec2 pass_uv;
	vec3 surfaceNormal;
	vec3 viewPos;
} o;

void main() {
	o.worldPos = (u_transformationMatrix * vec4(position, 1.0)).xyz;
	o.viewPos = normalize((inverse(u_viewMatrix) * vec4(0.0, 0.0, 0.0, 0.0)).xyz);
	o.pass_uv = uv;
	o.surfaceNormal = mat3(transpose(inverse(u_transformationMatrix))) * normal;

	gl_Position = u_projectionMatrix * u_viewMatrix * vec4(o.worldPos, 1.0);
}
