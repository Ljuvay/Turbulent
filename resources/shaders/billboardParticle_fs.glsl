#version 330 core

out vec4 FragColor;

in vec3 vColor;
in vec2 vUV;
in float vRadius;

void main()
{
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
