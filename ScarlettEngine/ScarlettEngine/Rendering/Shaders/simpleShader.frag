#version 450

layout(location = 0)
out vec4 color;

layout(set = 1, binding = 0)
uniform sampler2D textureSampler;

layout(location = 0)
in vec3 fragColor;
layout(location = 1)
in vec2 fragUv;

void main()
{
    color = texture(textureSampler, fragUv) * vec4(fragColor * 0.5, 1.0);
}