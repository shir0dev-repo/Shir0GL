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

layout (std140) uniform Lights {
	DirectionalLight sunlight;
	PointLight pointLights[MAX_POINT_LIGHTS];
	SpotLight spotLights[MAX_SPOT_LIGHTS];
};

in v2f {
	vec3 worldPos;
	vec2 pass_uv;
	vec3 surfaceNormal;
	vec3 viewPos;
} IN;



uniform sampler2D _MainTex;

uniform float ambientReflection;
uniform float specularReflection;
uniform float diffuseReflection;
uniform float shininess;

out vec4 FragColor;

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 attenuate(float distance, float radius, vec3 inColor, float falloff);

void main() {
	vec3 norm = normalize(IN.surfaceNormal);
	vec3 viewDir = normalize(IN.viewPos - IN.worldPos);
	
	vec3 result = calcDirLight(sunlight, norm, viewDir);

	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		result += calcPointLight(pointLights[i], norm, IN.worldPos, viewDir);
	}

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
		result += calcSpotLight(spotLights[i], norm, IN.worldPos, viewDir);
	}

	FragColor = (clamp(vec4(result, 1.0), 0.0, 1.0));
}

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction.xyz);
	
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient = ambientReflection * vec3(texture(_MainTex, IN.pass_uv));
	vec3 diffuse = light.diffuse.xyz * light.diffuse.w * diff * diffuseReflection * vec3(texture(_MainTex, IN.pass_uv));
	vec3 specular = light.specular.xyz * light.specular.w * spec * specularReflection * vec3(texture(_MainTex, IN.pass_uv));
	return (ambient + diffuse + spec);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position.xyz - fragPos);
	
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	
	float distance = length(light.position.xyz - fragPos);
	

	vec3 ambient = ambientReflection * vec3(texture(_MainTex, IN.pass_uv));
	vec3 diffuse = light.diffuse.xyz * light.diffuse.w * diff * diffuseReflection * vec3(texture(_MainTex, IN.pass_uv));
	vec3 specular = light.specular.xyz * light.specular.w * spec * specularReflection * vec3(texture(_MainTex, IN.pass_uv));
	
	float attenuation = smoothstep(light.attenuation.w, 0, distance);
	ambient *= attenuation;
	diffuse *=  attenuation;
	specular *= attenuation;
	return (clamp(ambient + diffuse + specular, 0.0, 1.0));
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position.xyz - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	float distance = length(light.position.xyz - fragPos);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * distance + light.attenuation.z * (distance * distance));

	float theta = dot(lightDir, normalize(-light.direction.xyz));
	float epsilon = light.innerAngle - light.outerAngle;
	float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);

	vec3 ambient = ambientReflection * vec3(texture(_MainTex, IN.pass_uv));
	vec3 diffuse = light.diffuse.xyz * light.diffuse.w * diff * diffuseReflection * vec3(texture(_MainTex, IN.pass_uv));
	vec3 specular = light.specular.xyz * light.specular.w * spec * specularReflection * vec3(texture(_MainTex, IN.pass_uv));
	ambient *= attenuation * intensity;
	specular *= attenuation * intensity;
	diffuse *= attenuation * intensity;

	return (clamp(ambient + diffuse + specular, 0.0, 1.0));
}

vec3 attenuate(float distance, float radius, vec3 inColor, float falloff) {
	float s = distance / radius;

	if (s >= 1.0) {
		return vec3(0.0);
	}

	float s2 = s * s;
	return inColor * ((1 - s2) * (1 - s2)) / (1 + falloff * s);
}