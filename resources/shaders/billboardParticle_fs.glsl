#version 330 core

out vec4 FragColor;

uniform vec3 Pposition;
uniform vec3 color;

in vec2 vUV;

void main()
{
    float vRadius = 1.0f;
    vec3 vColor = color;

    float borderPercentage = 0.1; // 0.0-1.0

    float dist = length(vUV);
    float borderLength = vRadius * borderPercentage;

    if(dist > vRadius) discard;
    if(dist > vRadius - borderLength){
        FragColor = vec4(0.0,0.0,0.0,1.0);
    }else{
        FragColor = vec4(vColor, 1.0);
    }
}
