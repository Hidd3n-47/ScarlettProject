#pragma once

#include <cmath>
#include <numbers>

namespace ScarlettMath
{

class Trig
{
public:
    [[nodiscard]] inline static float Sin(const float angleRadians)
    {
        return std::sin(angleRadians);
    }

    [[nodiscard]] inline static float Cos(const float angleRadians)
    {
        return std::cos(angleRadians);
    }

    [[nodiscard]] inline static float Asin(const float value)
    {
        return std::asin(value);
    }

    [[nodiscard]] inline static float Acos(const float value)
    {
        return std::acos(value);
    }

    [[nodiscard]] inline static float Atan(const float value)
    {
        return std::atan(value);
    }

    [[nodiscard]] inline static float CorrectAngleTo0To2PiRange(float angle)
    {
        while (angle < 0.0f || angle >= 2 * std::numbers::pi_v<float>)
        {
            if (angle >= 2 * std::numbers::pi_v<float>)
            {
                angle -= 2 * std::numbers::pi_v<float>;
            }
            else if (angle < 0.0f)
            {
                angle += 2 * std::numbers::pi_v<float>;
            }
        }

        return angle;
    }
};

} // Namespace ScarlettMath.
