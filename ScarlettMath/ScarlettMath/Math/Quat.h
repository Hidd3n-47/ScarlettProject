#pragma once

#include <numbers>

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
    explicit Quat(const Vec3& point);
    /**
     * @brief: Construct a quaternion to represent a rotation about a given axis.
     * @param angleRadians The angle of rotation about the axis.
     * @param axis The axis the rotation is about.
     */
    explicit Quat(const float angleRadians, const Vec3& axis);
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
    static Vec3 RotatePoint(const Vec3& point, const Quat& quaternion);

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

    /**
     * @brief Get the Yaw, Pitch and Roll rotations for the quaternion.
     * @remark Angles are return measured in radians.
     * @param yaw The angle representing the rotation in the 'yaw' axis (i.e. y-axis).
     * @param pitch The angle representing the rotation in the 'pitch' axis (i.e. x-axis).
     * @param roll The angle representing the rotation in the 'roll' axis (i.e. z-axis).
     */
    inline void GetYawPitchRoll(float& yaw, float& pitch, float& roll) const { yaw = mYaw; pitch = mPitch; roll = mRoll; }

    /**
     * @brief Set the Yaw, Pitch and Roll rotations for the quaternion.
     * @remark Angles are measured in radians.
     * @param yaw The angle representing the rotation in the 'yaw' axis (i.e. y-axis).
     * @param pitch The angle representing the rotation in the 'pitch' axis (i.e. x-axis).
     * @param roll The angle representing the rotation in the 'roll' axis (i.e. z-axis).
     */
    inline void SetYawPitchRoll(const float yaw, const float pitch, const float roll) { *this = Quat { yaw, pitch, roll }; }

    inline void operator*=(const Quat& rhs)
    {
        Multiply(*this, rhs, mX, mY, mZ, mW);

        mYaw   += rhs.mYaw;
        mPitch += rhs.mPitch;
        mRoll  += rhs.mRoll;
    }

    inline Quat operator*(const Quat& rhs) const
    {
        float x, y, z, w;
        Multiply(*this, rhs, x, y, z, w);

        Quat result { w, x, y, z };
        result.mYaw     = mYaw   + rhs.mYaw;
        result.mPitch   = mPitch + rhs.mPitch;
        result.mRoll    = mRoll  + rhs.mRoll;

        return result;
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
     * @brief Get the rotation needed to rotate a vector, \c vectorA, to another vector, \c vectorB.
     * @param vectorA The normalised original vector direction that rotated to a certain vector direction. Ensure that the vector is normalised before use.
     * @param vectorB The normalised vector direction that the original vector is being rotated to. Ensure that the vector is normalised before use.
     * @return The rotation needed to rotate the original vector to the direction of the other.
     */
    inline static Quat GetRotationToRotateVectorToVector(const Vec3& vectorA, const Vec3& vectorB)
    {
        // Todo could do with adding some tests for this.
        const float dot = Dot(vectorA, vectorB);

        if (IsEqualTo(dot, 1.0f))
        {
            return Identity();
        }

        // Todo when writing tests see if this case is needed or if the optimised method will cover this case.
        if (IsEqualTo(dot, -1.0f))
        {
            // Try to find an orthogonal vector to VectorA.
            Vec3 orthogonal = Cross({1, 0, 0 }, vectorA);

            // If the magnitude of the orthogonal vector is close to zero, try a different basis vector to check for orthogonality.
            // This would occur if vectorA = (1,0,0), in which case (0,1,0) will guarantee give non-zero cross product.
            if (IsEqualTo(Magnitude(orthogonal), 0.0f))
            {
                orthogonal = Cross({ 0, 1, 0 }, vectorA);
            }

            orthogonal = Normalize(orthogonal);

            return Quat(std::numbers::pi_v<float>, orthogonal);
        }

        const Vec3 axis = Cross(vectorA, vectorB);

        const float angle = std::sqrt((1 + dot) * 2);
        const float inverseAngle = 1.0f / angle;

        return Quat { -angle * 0.5f, axis.x * inverseAngle, axis.y * inverseAngle, axis.z * inverseAngle };
    }

    /**
     * Get a vector 4 representation of the quaternion.
     * @return A vector 4 representation of the quaternion as (w, x, y, z), or, (w, *v*).
     */
    [[nodiscard]] inline Vec4 ToVector4() const { return { mW, mX, mY, mZ }; }

    /**
     * @breif Get an identity quaternion.
     * @return An identity quaternion.
     */
    [[nodiscard]] static Quat Identity() { return Quat{ 1.0f, 0.0f, 0.0f, 0.0f }; }
private:
    float mW = 0.0f, mX = 0.0f, mY = 0.0f, mZ = 0.0f;

    float mYaw = 0.0f, mPitch = 0.0f, mRoll = 0.0f;

    inline static void Multiply(const Quat lhs, const Quat rhs, float& x, float& y, float& z, float& w)
    {
        x = lhs.mW * rhs.mX + lhs.mX * rhs.mW + lhs.mY * rhs.mZ - lhs.mZ * rhs.mY;
        y = lhs.mW * rhs.mY + lhs.mY * rhs.mW + lhs.mZ * rhs.mX - lhs.mX * rhs.mZ;
        z = lhs.mW * rhs.mZ + lhs.mZ * rhs.mW + lhs.mX * rhs.mY - lhs.mY * rhs.mX;
        w = lhs.mW * rhs.mW - lhs.mX * rhs.mX - lhs.mY * rhs.mY - lhs.mZ * rhs.mZ;
    }
};

}
