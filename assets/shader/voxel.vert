#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in uint voxelType;

uniform ivec3 chunkSize;
uniform vec3 chunkCoord;

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}
// Compound versions of the hashing algorithm I whipped together.
uint hash( uvec2 v ) { return hash( v.x ^ hash(v.y)                         ); }
uint hash( uvec3 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash( uvec4 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

float rand( float x ) { return floatConstruct(hash(floatBitsToUint(x))); }
float rand( vec2  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float rand( vec3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float rand( vec4  v ) { return floatConstruct(hash(floatBitsToUint(v))); }

layout (std140, column_major) uniform Matrices 
{
	mat4 u_viewMatrix;
	mat4 u_projectionMatrix;
};

out vec3 voxelColor;

void main() {
	//(z * CHUNK_SIZE_X * CHUNK_SIZE_Y) + (y * CHUNK_SIZE_X) + x;
	uint idx = gl_InstanceID;
	float z = float(idx / (chunkSize.x * chunkSize.y));
	idx -= int(z * chunkSize.x * chunkSize.y);
	
	float y = float(idx / chunkSize.x);
	float x = float(idx % chunkSize.x);
	vec3 positionInChunk = vec3(x, y, z) + chunkCoord;
	
	gl_Position = u_projectionMatrix * u_viewMatrix * vec4((positionInChunk + position), 1.0);
	voxelColor = vec3(rand(positionInChunk));

}