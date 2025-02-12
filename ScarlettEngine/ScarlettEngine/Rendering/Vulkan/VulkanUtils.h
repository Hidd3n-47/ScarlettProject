#pragma once

#define VK_CHECK(x, y)                                                  \
    do                                                                  \
    {                                                                   \
        VkResult result = x;                                            \
        if(result != VK_SUCCESS)                                        \
        {                                                               \
            SCARLETT_FLOG("{0}: {1}", y, static_cast<int>(result));     \
            throw std::runtime_error(y);                                \
        }                                                               \
    } while(false)                                                      