#version 330 core

#define MAX_LIGHTS 8

struct LightSource
{
    vec3 position;
    vec3 direction;
    vec3 color;
    float strength;
};

uniform LightSource lights[MAX_LIGHTS];
uniform int numLights;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform vec3 viewPos;
uniform sampler2D textures[4];
uniform bool hasTexture;
uniform vec2 tiling;

out vec4 FragColor;

in vec3 vPos;
in vec3 vNorm;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D shadowMap;
in vec4 FragPosLightSpace;
uniform int shadowLightIndex;

float calcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0) return 0.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.0001 * (1.0 - dot(normal, lightDir)), 0.0001);

    return currentDepth - bias > closestDepth ? 1.0 : 0.0;
}

void main()
{
    vec3 result = ambient;

    vec3 norm = normalize(vNorm);
    vec3 viewDir = normalize(viewPos - FragPos);

    float lightFade = 0.00001;
    float lightFadeAcc = 0.000001;

    for(int i = 0; i < numLights; i++)
    {
       float dist = length(lights[i].position - FragPos);
       float attenuation = 1.0 / (1.0 + lightFade * dist + lightFadeAcc * dist * dist);

       vec3 lightDir = (i == shadowLightIndex) ? normalize(-lights[i].direction) : normalize(lights[i].position - FragPos);

       float diff = max(dot(norm, lightDir), 0.0);
       vec3 diffAccum = diff * lights[i].color * lights[i].strength * attenuation;

       float specularStrength = shininess;

       vec3 halfDir = normalize(lightDir + viewDir);
       float spec = pow(max(dot(norm, halfDir), 0.0), shininess);

       vec3 specAccum = lights[i].strength * spec * lights[i].color * specular * attenuation;

       float shadow = 0.0;
       if (i == shadowLightIndex) {
       shadow = calcShadow(FragPosLightSpace, norm, lightDir);
       }

       result += (diffAccum + specAccum) * (1.0 - shadow);
       
    }
    
    vec3 albedo = hasTexture ? texture(textures[0], TexCoord * tiling).rgb : diffuse;
    FragColor = vec4(result * albedo, 1.0);
}