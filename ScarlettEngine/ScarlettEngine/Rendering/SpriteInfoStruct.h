#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

namespace Scarlett {

struct SpriteInfoStruct
{
    /*glm::vec4	color = glm::vec4(1.0f);
    glm::vec2 	translation = glm::vec2(0.0f);
    glm::vec2 	scale = glm::vec2(1.0f);
    float 		rotation = 0.0f;*/
    glm::vec4 color = glm::vec4(1.0f);

    glm::mat4 model = glm::mat4(1.0f);
};

} // Namespace Scarlett.