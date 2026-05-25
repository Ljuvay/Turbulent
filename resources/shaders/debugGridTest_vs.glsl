#version 330 core

uniform mat4 view;
uniform mat4 projection;

out vec3 nearPoint;
out vec3 farPoint;

vec3 unproject(float x, float y, float z)
{
    mat4 invVP = inverse(projection * view);
    vec4 unprojected = invVP * vec4(x, y, z, 1.0);
    return unprojected.xyz / unprojected.w;

}

void main()
{
    vec2 verts[3] = vec2[](
        vec2(-1, -1),
        vec2(3, -1),
        vec2(-1, 3)
    );
    vec2 p = verts[gl_VertexID];
    nearPoint = unproject(p.x, p.y, 0.0);
    farPoint = unproject(p.x, p.y, 1.0);
    gl_Position = vec4(p, 0.0, 1.0);
    
}