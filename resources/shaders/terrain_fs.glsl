#version 330 core

#define MAX_LIGHTS 8

struct LightSource
{
    vec3 position;
    vec3 color;
    float strength;
};

uniform LightSource lights[MAX_LIGHTS];
uniform int numLights;

uniform vec3 ambient;
uniform vec3 specular;
uniform float shininess;

uniform vec3 viewPos;

out vec4 FragColor;

in vec3 vPos;
in vec3 vNorm;
in vec3 FragPos;

void main()
{

	// Solid terrain color
	float hVal = (vPos.y + 13) / 220.0; // Min - Max of terrain height
	vec3 vColor;

	if(hVal < 0.15){vColor = vec3(0.75, 0.7, 0.5);} //Sand
	else if(hVal <= 0.3){vColor = vec3(0.12, 0.25, 0.0);} //Grass
	else if(hVal <= 0.85){vColor = vec3(0.5);} //Stone
	else{vColor = vec3(1.0);}

	vec3 result = ambient;

	vec3 norm = normalize(vNorm);
	vec3 viewDir = normalize(viewPos - FragPos);

	float lightFade = 0.001;
    float lightFadeAcc = 0.000001;

	for (int i = 0; i < numLights; i++)
	{
	   float dist = length(lights[i].position - FragPos);
       float attenuation = 1.0 / (1.0 + lightFade * dist + lightFadeAcc * dist * dist);


		vec3 lightDir = normalize(lights[i].position - FragPos);

		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffAccum = diff * lights[i].color * lights[i].strength * attenuation;

		float specularStrength = shininess;

		vec3 halfDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(norm, halfDir), 0.0), shininess);

		vec3 specAccum = lights[i].strength * spec * lights[i].color * specular * attenuation;

		result += (diffAccum + specAccum);
	}

	
	FragColor = vec4(result * vColor, 1.0);
	//FragColor = vec4(normalize(vNorm) * 0.5 + 0.5, 1.0); //Debug normal colors
}

