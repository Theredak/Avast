#version 420

uniform vec3 uSceneAmbient = vec3(0.5, 0.5, 0.5);
uniform vec3 uLightPosition;// = vec3(3.0, 0.0, 0.0);
uniform vec3 uLightColor = vec3(1.0, 0.9, 0.8);
uniform float uLightSpecularExponent = 32.0;
uniform float uTime;

struct fogParams
{
	vec4 color;
	float constant;
	float linear;
	float quad;
};

uniform fogParams uFog = fogParams(vec4(0.5f, 0.5f, 0.5f, 0.0f), 0.0, 0.05, 0.0001);

layout(binding = 0) uniform sampler2D uTex;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

out vec4 outColor;

float calcFog()
{
	float distPos = length(pos);

	return min(1, distPos * distPos * uFog.quad + distPos * uFog.linear);
}

void main()
{
	vec2 texOffset = texcoord;

	vec4 albedoColor = texture(uTex, texOffset);
	outColor.rgb = albedoColor.rgb * uSceneAmbient; 
	outColor.a = albedoColor.a;

	// Fix length after rasterizer interpolates
	vec3 normal = normalize(norm);

	vec3 lightVec = uLightPosition - pos;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;

	float NdotL = dot(normal, lightDir);

	// If the normal is facing the light
	if (NdotL > 0.0)
	{
		// Normalized vector pointing towards the camera
		vec3 eye = normalize(-pos);
		
		// Calculate attenuation (falloff)
		// Add a small number to avoid divide by zero.
		float attenuation = 1.0 / (0.5 + dist * 2.0);

		// Calculate the diffuse contribution
		outColor.rgb += albedoColor.rgb * uLightColor * NdotL * attenuation;
		
		vec3 reflection = reflect(-lightDir, normal);
		
		float specularStrength = dot(reflection, eye);
		specularStrength = max(specularStrength, 0.0f); // don't let it fall before zero

		// Calculate the specular contribution
		outColor.rgb += texture(uTex, texOffset).rgb * uLightColor * pow(specularStrength, uLightSpecularExponent) * attenuation;
	}

	//outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//float fogAmount = calcFog();
	//if(fogAmount == 1)
	//{
	//	discard;
	//}
	//
	//outColor.rgb = mix(outColor.rgb, uFog.color.rgb, fogAmount);
	outColor.rgb += texture(uTex, texOffset).rgb * uSceneAmbient;	
}