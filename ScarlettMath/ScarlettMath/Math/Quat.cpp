#include "Quat.h"

#include "Trig.h"

namespace ScarlettMath
{

Quat::Quat(const Vec3 point)
    : mX (point.x)
    , mY (point.y)
    , mZ (point.z)
{
    // Empty.
}

Quat::Quat(const float angleRadians, const Vec3 axis)
{
    const float halfAngle = angleRadians * 0.5f;

    mX = axis.x * Trig::Sin(halfAngle);
    mY = axis.y * Trig::Sin(halfAngle);
    mZ = axis.z * Trig::Sin(halfAngle);

    mW = Trig::Cos(halfAngle);
}

Quat::Quat(const float yawRadians, const float pitchRadians, const float rollRadians)
    : mYaw   (Trig::CorrectAngleTo0To2PiRange(yawRadians))
    , mPitch (Trig::CorrectAngleTo0To2PiRange(pitchRadians))
    , mRoll  (Trig::CorrectAngleTo0To2PiRange(rollRadians))
{
    const Quat x { pitchRadians  , { 1.0f, 0.0f, 0.0f } };
    const Quat y { yawRadians    , { 0.0f, 1.0f, 0.0f } };
    const Quat z { rollRadians   , { 0.0f, 0.0f, 1.0f } };

    const Quat rotation = z * y * x;
    mW = rotation.mW;
    mX = rotation.mX;
    mY = rotation.mY;
    mZ = rotation.mZ;
}

Quat::Quat(const float w, const float x, const float y, const float z)
    : mW(w), mX(x), mY(y), mZ(z)
{
    // Empty.
}

void Quat::Normalise()
{
    if (const float length = Sqrt(mX * mX + mY * mY + mZ * mZ + mW * mW); length != 0.0f)
    {
        mX /= length;
        mY /= length;
        mZ /= length;
        mW /= length;
    }
}

Vec3 Quat::RotatePoint(const Vec3 point, const Quat& quaternion)
{
    const Quat pointQuat { point };
    return (quaternion * pointQuat * quaternion.Inverse()).GetAxis();
}

Quat Quat::Inverse() const
{
    return Quat{ mW, -mX, -mY, -mZ };
}

Mat4 Quat::GetRotationMatrix() const
{
    return {
        1.f - 2.f * mY * mY - 2.f * mZ * mZ  ,        2.f * mX * mY - 2.f * mW * mZ  ,        2.f * mX * mZ + 2.f * mW * mY  ,  0.f,
        2.f * mX * mY + 2.f * mW * mZ        ,  1.f - 2.f * mX * mX - 2.f * mZ * mZ  ,        2.f * mY * mZ - 2.f * mW * mX  ,  0.f,
        2.f * mX * mZ - 2.f * mW * mY        ,        2.f * mY * mZ + 2.f * mW * mX  ,  1.f - 2.f * mX * mX - 2.f * mY * mY  ,  0.f,
        0.f                                  ,                                  0.f  ,                                  0.f  ,  1.f
    };
}

// =====================================================================================================================
// Left here as a note if it ever comes back to getting angles from the quaternion values.
// This works the only thing that needs to be accounted for is when cosYaw == 0.0f, i.e. at pi / 2 and 3/2*pi
// ---------------------------------------------------------------------------------------------------------------------
// void Quat::GetYawPitchRoll(float& yaw, float& pitch, float& roll) const
// {
//     yaw = Asin(2 * (mW * mY - mX * mZ));
//
//     const float cosYaw = Cos(yaw);
//
//     pitch = Asin((2 * (mY * mZ + mW * mX)) / cosYaw);
//     roll  = Asin((2 * (mX * mY + mW * mZ)) / cosYaw);
// }
// =====================================================================================================================

} // Namespace ScarlettMath.
