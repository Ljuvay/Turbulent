// Billboard shader logic Loosely based off tutorial, "https://ogldev.org/www/tutorial27/tutorial27.html"

#version 330 core

uniform mat4 view;
uniform mat4 projection;
uniform vec3 Pposition;

layout(location = 0) in vec3 position;  
layout(location = 1) in vec3 normal;    
layout(location = 2) in vec2 uv;

out vec3 vColor;
out vec2 vUV;
out float vRadius;

void main()
{

    vec3 camRight = normalize(vec3(view[0][0], view[1][0], view[2][0]));
    vec3 camUp    = normalize(vec3(view[0][1], view[1][1], view[2][1]));

    vec3 worldPos = Pposition
        + camRight * position.x * (2)
        + camUp    * position.y * (2);

    gl_Position = projection * view * vec4(worldPos, 1.0);

    vUV = (uv - 0.5) * 2;
}