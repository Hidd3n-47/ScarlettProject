#version 450

layout(location = 0)
in vec2 position;

layout(push_constant) 
uniform Sprite 
{
	vec4 color;
	mat4 model;
} sprite;

void main()
{
    gl_Position = sprite.model * vec4(position, 0.0, 1.0);
}