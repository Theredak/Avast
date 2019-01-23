#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform float uTime;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

out vec3 texcoord;
out vec3 norm;
out vec3 pos;
out float height;

void main()
{
	texcoord = vec3(in_uv, 0.0f);
	texcoord.y = 1 - texcoord.y;
	texcoord.x = sin(uTime + texcoord.y * 3) * 0.1 + texcoord.x;
	//texcoord.y = sin(uTime + texcoord.x * 4)* 0.1+ texcoord.y;
	
	vec4 world_space_pos = uModel * vec4(in_vert, 1.0);

	float height = sin((world_space_pos.z + world_space_pos.x + uTime * 5) / 8.0f) * 1.0f;
	
	// Apply the delta to just the y coordinate.
	world_space_pos.y += height;

	// Complete the transformations on our modified position.
	gl_Position = uProj * uView * world_space_pos;
}