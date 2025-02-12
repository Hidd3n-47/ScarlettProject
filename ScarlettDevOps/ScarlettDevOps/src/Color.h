#pragma once

#include <imgui.h>

class Color
{
public:
    inline Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
            : mR(static_cast<float>(r) / 255.0f)
            , mG(static_cast<float>(g) / 255.0f)
            , mB(static_cast<float>(b) / 255.0f)
            , mA(static_cast<float>(a) / 255.0f)
        {
            // Empty.
        }

    inline ImVec4 ImColor() const { return { mR, mG, mB, mA }; }

private:
    float mR, mG, mB, mA;
};

namespace Colors
{
    inline Color bgPrimary{ 18, 18, 18 };
    inline Color bgSecondary{ 25, 25, 25 };
    inline Color textSecondary{ 255, 65, 130 };
    inline Color button{ 26, 192, 42 };
    inline Color buttonHover{ 33, 160, 46 };
    inline Color redColor{ 222, 26, 26 };
}