#pragma once

#include "Math.h"

namespace ScarlettMath
{

/**
 * @class Quat: A class to represent quaternions which are used for rotations.
 */
class Quat
{
public:
    Quat() = default;
    /**
     * @brief: Construct a quaternion to represent a point in space.<br/>
     * This is useful when trying to rotate a point as quaternion multiplication can be used.
     * @param point The point in space that is being represented in quaternion form.
     */
    explicit Quat(const Vec3 point);
    /**
     * @brief Construct a quaternion to represent a given rotation about a given axis.
     * @param angleRadians The angle of rotation (in radians) that the quaternion represents.
     * @param axis The axis the rotation is about.
     */
    explicit Quat(const float angleRadians, const Vec3 axis);
    /**
     * @brief Construct a quaternion to represent a rotation from the given Yaw, Pitch and Roll rotation.
     * @param yawRadians The angle of rotation (in radians) about the z-axis.
     * @param pitchRadians The angle of rotation (in radians) about the y-axis.
     * @param rollRadians The angle of rotation (in radians) about the x-axis.
     */
    explicit Quat(const float yawRadians, const float pitchRadians, const float rollRadians);
    /**
     * @brief Construct a quaternion from given values of a quaternion.
     * @param w The w component of the quaternion, also known as the real value.
     * @param x The x component of the quaternion, also known as the i coefficients in the imaginary numbers.
     * @param y The y component of the quaternion, also known as the j coefficients in the imaginary numbers.
     * @param z The z component of the quaternion, also known as the k coefficients in the imaginary numbers.
     */
    explicit Quat(const float w, const float x, const float y, const float z);

    /**
     * @brief Normalise the quaternion to have a length of 1.
     */
    void Normalise();

    /**
     * @brief Rotate a given point by a given rotation.
     * @param point The point being rotated
     * @param quaternion The quaternion used to rotate the point.
     * @return The rotated point.
     */
    static Vec3 RotatePoint(const Vec3 point, const Quat quaternion);

    /**
     * @brief Get if the quaternion is an identity quaternion.
     * @return \c True if identity quaternion, \c false otherwise.
     */
    [[nodiscard]] inline bool IsIdentity() const { return IsEqualTo(mX, mY) && IsEqualTo(mY, mZ) && IsEqualTo(mZ, 0.0f) && IsEqualTo(mW, 1.0f); }
    /**
     * @breif Get the axis of rotation. This is also the coefficients of the imaginary parts.
     * @return A vector representing the coefficients of the imaginary parts, also known as the rotation axis.
     */
    [[nodiscard]] inline Vec3 GetAxis() const { return { mX, mY, mZ }; }

    /**
     * @brief Get the inverse of the quaternion.
     * @remark Note this should only be done on unit quaternions as it uses the conjugate for optimisations reasons.
     * @return The inverse of this quaternion.
     */
    [[nodiscard]] Quat Inverse() const;
    /**
     * @breif Get the matrix representation of the rotation.
     * @return The matrix which represents the rotation of the quaternion.
     */
    [[nodiscard]] Mat4 GetRotationMatrix() const;

    inline void operator*=(const Quat& rhs)
    {
        Multiply(*this, rhs, mX, mY, mZ, mW);
    }

    inline Quat operator*(const Quat& rhs) const
    {
        float x, y, z, w;
        Multiply(*this, rhs, x, y, z, w);

        return Quat{ w, x, y, z };
    }

    inline bool operator==(const Quat& rhs) const
    {
        return IsEqualTo(mX, rhs.mX) &&
               IsEqualTo(mY, rhs.mY) &&
               IsEqualTo(mZ, rhs.mZ) &&
               IsEqualTo(mW, rhs.mW);
    }

    inline bool operator==(const Quat& rhs)
    {
        return IsEqualTo(mX, rhs.mX) &&
               IsEqualTo(mY, rhs.mY) &&
               IsEqualTo(mZ, rhs.mZ) &&
               IsEqualTo(mW, rhs.mW);
    }

    /**
     * @breif Get an identity quaternion.
     * @return An identity quaternion.
     */
    [[nodiscard]] static Quat Identity() { return Quat{ 1.0f, 0.0f, 0.0f, 0.0f }; }
private:
    float mW = 0.0f, mX = 0.0f, mY = 0.0f, mZ = 0.0f;

    inline static void Multiply(const Quat lhs, const Quat rhs, float& x, float& y, float& z, float& w)
    {
        x = lhs.mW * rhs.mX + lhs.mX * rhs.mW + lhs.mY * rhs.mZ - lhs.mZ * rhs.mY;
        y = lhs.mW * rhs.mY - lhs.mX * rhs.mZ + lhs.mY * rhs.mW + lhs.mZ * rhs.mX;
        z = lhs.mW * rhs.mZ + lhs.mX * rhs.mY - lhs.mY * rhs.mX + lhs.mZ * rhs.mW;
        w = lhs.mW * rhs.mW - lhs.mX * rhs.mX - lhs.mY * rhs.mY - lhs.mZ * rhs.mZ;
    }
};

}
