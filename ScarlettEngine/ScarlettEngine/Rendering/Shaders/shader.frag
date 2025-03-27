#version 450

layout(location = 0)
out vec4 color;

layout(push_constant)
uniform Sprite
{
    vec4 color;
    mat4 model;
} sprite;

void main()
{
    color = vec4(sprite.color);
}
