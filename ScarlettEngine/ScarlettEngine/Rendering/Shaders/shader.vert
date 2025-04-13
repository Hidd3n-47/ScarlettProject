#version 450

layout(location = 0)
in vec3 position;

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
}
