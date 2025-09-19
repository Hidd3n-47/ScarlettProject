#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0)
out vec4 color;

layout(location = 0)
in vec3 fragNormal;
layout(location = 1)
in vec2 textCoord;

layout(push_constant)
uniform ModelInfo
{
    // Transform.
    mat4 view;
    mat4 proj;
    mat4 model;

    // Color.
    vec4 baseColor;

    // Material.
    uint albedoTextureIndex;

    // uint normalTextureIndex;
    // uint roughnessTextureIndex;
    // uint metallicTextureIndex;

    // vec4 baseColorFactor;
    // float roughnessFactor;
    // float metallicFactor;
} uModelInfo;

layout(set = 0, binding = 0) uniform sampler2D uAlbedoTextures[];

// layout(set = 1, binding = 1) uniform sampler2D normalTextures[];
// layout(set = 1, binding = 2) uniform sampler2D roughnessTextures[];
// layout(set = 1, binding = 3) uniform sampler2D metallicTextures[];

void main()
{
    vec3 lightDir = normalize(vec3(1,1,0));
    float intensity = max(dot(fragNormal, lightDir), 0.2);

    vec4 textureSample = texture(uAlbedoTextures[uModelInfo.albedoTextureIndex], textCoord);

    // This is with some basic lighting calculations.
    //color = vec4(vec4(intensity, intensity, intensity, 1.0f) * uModelInfo.baseColor * textureSample);
    color = vec4(uModelInfo.baseColor * textureSample);
}
