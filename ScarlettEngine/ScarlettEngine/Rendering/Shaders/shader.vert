#version 450

layout(location = 0)
in vec3 position;
layout(location = 1)
in vec3 normal;
layout(location = 2)
in vec2 inTextCoord;

layout(location = 0)
out vec3 fragNormal;
layout(location = 1)
out vec2 textCoord;

layout(push_constant)
uniform Sprite
{
    vec4 color;
    mat4 view;
    mat4 proj;
    mat4 model;
} sprite;

void main()
{
    gl_Position = sprite.proj * sprite.view * sprite.model * vec4(position, 1.0);
    gl_Position.y *= -1.0;

    fragNormal = normalize(transpose(inverse(mat3(sprite.model))) * normal);
    textCoord = inTextCoord;
}
