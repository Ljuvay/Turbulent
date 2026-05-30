#version 330 core

//uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

in vec3 vPos;
in vec3 vNorm;
in vec3 FragPos;
flat in vec3 vColor;

void main()
{
    vec3 lightColor = vec3(1.0, 0.5, 1.0);
    vec3 lightPos = vec3(0.0, 600.0, 200.0);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(vNorm);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);

    // Phong
    //vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // Blinn-Phong
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfDir), 0.0), 32.0);

    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * vColor;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(normalize(vNorm) * 0.5 + 0.5, 1.0);
}

