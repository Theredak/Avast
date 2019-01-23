#version 420

uniform vec3 uSceneAmbient = vec3(0.2, 0.2, 0.2);
uniform float uTime;

layout(binding = 0) uniform sampler2D uTex;

in vec3 texcoord;
in vec3 norm;
in vec3 pos;

out vec4 outColor;

void main()
{
	vec2 texOffset = vec2(texcoord);

	vec4 albedoColor = texture(uTex, vec2(texcoord));
	outColor.rgb = albedoColor.rgb * uSceneAmbient; 
	outColor.a = albedoColor.a;
		
	outColor.rgb += texture(uTex, texOffset).rgb;	
}