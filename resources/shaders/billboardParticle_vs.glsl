// Billboard shader logic Loosely based off tutorial, "https://ogldev.org/www/tutorial27/tutorial27.html"

#version 330 core

uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec2 aQuadPos;  
layout(location = 1) in vec3 aPos;    
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;
layout(location = 4) in vec3 aColor;
layout(location = 5) in float aRadius;

out vec3 vColor;
out vec2 vUV;
out float vRadius;

void main()
{

    vec3 camRight = normalize(vec3(view[0][0], view[1][0], view[2][0]));
    vec3 camUp    = normalize(vec3(view[0][1], view[1][1], view[2][1]));

    vec3 worldPos = aPos
        + camRight * aQuadPos.x * (aRadius)
        + camUp    * aQuadPos.y * (aRadius);

    gl_Position = projection * view * vec4(worldPos, 1.0);

    vRadius = aRadius;
    vColor = aColor;
    vUV = aQuadPos;
}