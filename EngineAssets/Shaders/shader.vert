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

void main()
{
    gl_Position = uModelInfo.proj * uModelInfo.view * uModelInfo.model * vec4(position, 1.0);
    gl_Position.y *= -1.0;

    fragNormal = normalize(transpose(inverse(mat3(uModelInfo.model))) * normal);
    textCoord = inTextCoord;
}
