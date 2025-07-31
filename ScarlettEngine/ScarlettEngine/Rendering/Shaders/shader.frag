#version 450

layout(location = 0)
out vec4 color;

layout(location = 0)
in vec3 fragNormal;
layout(location = 1)
in vec2 textCoord;

layout(binding = 0)
uniform sampler2D uTexture;

layout(push_constant)
uniform Sprite
{
    vec4 color;
    mat4 model;
} sprite;

void main()
{
    vec3 lightDir = normalize(vec3(1,1,0));
    float intensity = max(dot(fragNormal, lightDir), 0.2);

    vec4 textureSample = texture(uTexture, textCoord);

    // This is with some basic lighting calculations.
    //color = vec4(vec4(intensity, intensity, intensity, 1.0f) * sprite.color * textureSample);
    color = vec4(sprite.color * textureSample);
}
