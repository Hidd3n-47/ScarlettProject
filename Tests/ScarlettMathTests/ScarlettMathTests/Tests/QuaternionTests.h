#pragma once

#include <numbers>

#include <Math/Quat.h>

class QuaternionTests
{
public:
    explicit inline QuaternionTests(Scarlett::TestRegistry* testRegistry)
    {
        // Quaternions created correctly.
        testRegistry->AddTestCase("Quaternion Tests", "CorrectQuaternionForPiOverTwoRotationAboutZ", CorrectQuaternionForPiOverTwoRotationAboutZ);
        testRegistry->AddTestCase("Quaternion Tests", "CorrectQuaternionForPiOverFourRotationAboutAllAxes", CorrectQuaternionForPiOverFourRotationAboutAllAxes);

        // Quaternion returns correct rotation matrices.
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionReturnsCorrectRotationMatrix", QuaternionReturnsCorrectRotationMatrixWhenNoRotation);
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionReturnsCorrectRotationMatrixWhenRotateAboutX", QuaternionReturnsCorrectRotationMatrixWhenRotateAboutX);
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionReturnsCorrectRotationMatrixWhenRotateAboutY", QuaternionReturnsCorrectRotationMatrixWhenRotateAboutY);
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionReturnsCorrectRotationMatrixWhenRotateAboutZ", QuaternionReturnsCorrectRotationMatrixWhenRotateAboutZ);
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionReturnsCorrectRotationMatrixAtAnglesOfPiAndTwoPi", QuaternionReturnsCorrectRotationMatrixAtAnglesOfPiAndTwoPi);
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotation", RotationMatrixAndQuaternionResultInSameRotation);

        // Quaternion multiplication.
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionMultipliedByInverseIsIdentity", QuaternionMultipliedByInverseIsIdentity);
        testRegistry->AddTestCase("Quaternion Tests", "ComplexQuaternionMultipliedByInverseIsIdentity", ComplexQuaternionMultipliedByInverseIsIdentity);
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionMultipliedByQuaternion", QuaternionMultipliedByQuaternion);

        // Quaternion point rotation.
        testRegistry->AddTestCase("Quaternion Tests", "OriginDoesNotRotate", OriginDoesNotRotate);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotateAndUnRotate", PointRotateAndUnRotate);
        testRegistry->AddTestCase("Quaternion Tests", "UnitXRotationAboutX", UnitXRotationAboutX);
        testRegistry->AddTestCase("Quaternion Tests", "UnitYRotationAboutX", UnitYRotationAboutX);
        testRegistry->AddTestCase("Quaternion Tests", "UnitZRotationAboutX", UnitZRotationAboutX);
        testRegistry->AddTestCase("Quaternion Tests", "UnitXRotationAboutY", UnitXRotationAboutY);
        testRegistry->AddTestCase("Quaternion Tests", "UnitYRotationAboutY", UnitYRotationAboutY);
        testRegistry->AddTestCase("Quaternion Tests", "UnitZRotationAboutY", UnitZRotationAboutY);
        testRegistry->AddTestCase("Quaternion Tests", "UnitXRotationAboutZ", UnitXRotationAboutZ);
        testRegistry->AddTestCase("Quaternion Tests", "UnitYRotationAboutZ", UnitYRotationAboutZ);
        testRegistry->AddTestCase("Quaternion Tests", "UnitZRotationAboutZ", UnitZRotationAboutZ);
        testRegistry->AddTestCase("Quaternion Tests", "PointsOnXYRotatesCorrectly", PointsOnXYRotatesCorrectly);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutX", PointOneTwoThreeAboutX);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutY", PointOneTwoThreeAboutY);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutZ", PointOneTwoThreeAboutZ);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutXY", PointOneTwoThreeAboutXY);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutYZ", PointOneTwoThreeAboutYZ);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutXZ", PointOneTwoThreeAboutXZ);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutXYZ", PointOneTwoThreeAboutXYZ);
    }

    inline static bool CorrectQuaternionForPiOverTwoRotationAboutZ()
    {
        const ScarlettMath::Quat solution { 0.707107f, 0.0f,0.0f,0.707107f };

        const ScarlettMath::Quat quat(std::numbers::pi_v<float> * 0.5f, { 0.0f, 0.0f, 1.0f });

        return solution == quat;
    }

    inline static bool CorrectQuaternionForPiOverFourRotationAboutAllAxes()
    {
        const ScarlettMath::Quat solution { 0.844623f, 0.191342f, 0.46194f, 0.191342f };

        constexpr float piOverFour = std::numbers::pi_v<float> * 0.25f;
        const ScarlettMath::Quat quat { piOverFour, piOverFour, piOverFour };

        return solution == quat;
    }

    inline static bool QuaternionReturnsCorrectRotationMatrixWhenNoRotation()
    {
        constexpr ScarlettMath::Mat4 solution = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        const ScarlettMath::Quat quat( 0.0f, { 1.0f, 0.0f, 0.0f });

        return ScarlettMath::IsEqualTo(solution, quat.GetRotationMatrix());
    }

    inline static bool QuaternionReturnsCorrectRotationMatrixWhenRotateAboutX()
    {
        constexpr ScarlettMath::Mat4 solution = {
            1.0f,      0.0f,       0.0f, 0.0f,
            0.0f,      0.5f, -0.866025f, 0.0f,
            0.0f, 0.866025f,       0.5f, 0.0f,
            0.0f,      0.0f,       0.0f, 1.0f
        };

        const ScarlettMath::Quat quat(std::numbers::pi_v<float> / 3.0f, { 1.0f, 0.0f, 0.0f });

        return ScarlettMath::IsEqualTo(solution, quat.GetRotationMatrix());
    }

    inline static bool QuaternionReturnsCorrectRotationMatrixWhenRotateAboutY()
    {
        constexpr ScarlettMath::Mat4 solution = {
            0.866025f,  0.0f,      0.5f, 0.0f,
                 0.0f,  1.0f,      0.0f, 0.0f,
                -0.5f,  0.0f, 0.866025f, 0.0f,
                 0.0f,  0.0f,      0.0f, 1.0f
        };

        const ScarlettMath::Quat quat(std::numbers::pi_v<float> / 6.0f, { 0.0f, 1.0f, 0.0f });

        return ScarlettMath::IsEqualTo(solution, quat.GetRotationMatrix());
    }

    inline static bool QuaternionReturnsCorrectRotationMatrixWhenRotateAboutZ()
    {
        constexpr ScarlettMath::Mat4 solution = {
            0.0f, -1.0f, 0.0f, 0.0f,
            1.0f,  0.0f, 0.0f, 0.0f,
            0.0f,  0.0f, 1.0f, 0.0f,
            0.0f,  0.0f, 0.0f, 1.0f
        };

        const ScarlettMath::Quat quat(std::numbers::pi_v<float> * 0.5f, { 0.0f, 0.0f, 1.0f });

        return ScarlettMath::IsEqualTo(solution, quat.GetRotationMatrix());
    }

    inline static bool QuaternionReturnsCorrectRotationMatrixAtAnglesOfPiAndTwoPi()
    {
        constexpr ScarlettMath::Mat4 solutionPi = {
            -1.0f,  0.0f, 0.0f, 0.0f,
             0.0f, -1.0f, 0.0f, 0.0f,
             0.0f,  0.0f, 1.0f, 0.0f,
             0.0f,  0.0f, 0.0f, 1.0f
        };

        constexpr ScarlettMath::Mat4 solutionTwoPi = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        const ScarlettMath::Quat quatPi(std::numbers::pi_v<float>, { 0.0f, 0.0f, 1.0f });
        const ScarlettMath::Quat quatTwoPi(2.0f * std::numbers::pi_v<float>, { 0.0f, 0.0f, 1.0f });

        return ScarlettMath::IsEqualTo(solutionPi, quatPi.GetRotationMatrix()) &&
               ScarlettMath::IsEqualTo(solutionTwoPi, quatTwoPi.GetRotationMatrix());
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotation()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Mat4 pointMat   { 1.0f, 0.0f, 0.0f, 1.0f , 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 3.0f, 0.0f, 0.0f, 0.0f, 1.0f };
        constexpr ScarlettMath::Vec3 solution   { 1.207107f,0.207107f,3.535534f };

        const float pitch = ScarlettMath::Radians(45.0f);
        const float roll  = ScarlettMath::Radians(45.0f);
        const ScarlettMath::Quat rotation { 0.0f, pitch, roll };
        const ScarlettMath::Vec3 resultQuat = ScarlettMath::Quat::RotatePoint(point, rotation);
        const ScarlettMath::Mat4 resultMat  = pointMat * rotation.GetRotationMatrix();
        const ScarlettMath::Vec3 resultMatAnswer = { resultMat[0][3], resultMat[1][3], resultMat[2][3] };

        return ScarlettMath::IsEqualTo(resultQuat, solution) && ScarlettMath::IsEqualTo(resultMatAnswer, solution);
    }

    inline static bool QuaternionMultipliedByInverseIsIdentity()
    {
        ScarlettMath::Quat quat { std::numbers::pi_v<float> * 0.5f, { 0.0f, 1.0f, 0.0f } };
        quat *= quat.Inverse();

        return quat.IsIdentity();
    }

    inline static bool ComplexQuaternionMultipliedByInverseIsIdentity()
    {
        ScarlettMath::Quat quat { std::numbers::pi_v<float> * 0.5f, { 0.1f, -0.2f, 0.3f } };
        quat.Normalise();

        quat *= quat.Inverse();

        return quat.IsIdentity();
    }

    inline static bool QuaternionMultipliedByQuaternion()
    {
        constexpr float angle1 = std::numbers::pi_v<float> * 0.5f;
        constexpr float angle2 = std::numbers::pi_v<float> / 3.0f;

        const ScarlettMath::Quat quat1 { angle1, { 1.0f, 0.0f, 0.0f } };
        const ScarlettMath::Quat quat2 { angle2, { 1.0f, 0.0f, 0.0f } };

        const ScarlettMath::Quat solution { angle1 + angle2, { 1.0f, 0.0f, 0.0f } };

        const ScarlettMath::Quat answer = quat1 * quat2;

        return solution == answer;
    }

    inline static bool OriginDoesNotRotate()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ ScarlettMath::Radians(20.0f), ScarlettMath::Radians(30.0f), ScarlettMath::Radians(40.0f) });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    static ScarlettMath::Vec3 RotatePoint(const ScarlettMath::Vec3 point, const float yawDeg, const float pitchDeg, const float rollDeg)
    {
        const float yaw   = ScarlettMath::Radians(yawDeg);
        const float pitch = ScarlettMath::Radians(pitchDeg);
        const float roll  = ScarlettMath::Radians(rollDeg);
        return ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ yaw, pitch, roll });
    }

    inline static bool PointRotateAndUnRotate()
    {
        constexpr ScarlettMath::Vec3 point      { 1.2f, 2.4f, 3.6f };

        const float yaw   = ScarlettMath::Radians(15.0f);
        const float pitch = ScarlettMath::Radians(25.0f);
        const float roll  = ScarlettMath::Radians(35.0f);
        const ScarlettMath::Quat rotation = ScarlettMath::Quat{ yaw, pitch, roll };

        const ScarlettMath::Vec3 rotate = ScarlettMath::Quat::RotatePoint(point, rotation);
        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(rotate, rotation.Inverse());

        return ScarlettMath::IsEqualTo(result, point);
    }

    inline static bool UnitXRotationAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0f, 90.0f, 0.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitYRotationAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 1.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, 0.0f, 1.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0f, 90.0f, 0.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitZRotationAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f,  0.0f, 1.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, -1.0f, 0.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0f, 90.0f, 0.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitXRotationAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 0.0f,  0.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, 0.0f, -1.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 90.0f, 0.0f, 0.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitYRotationAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 1.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = RotatePoint(point, 90.0f, 0.0f, 0.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitZRotationAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 0.0f, 1.0f };
        constexpr ScarlettMath::Vec3 solution   { 1.0f, 0.0f, 0.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 90.0f, 0.0f, 0.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitXRotationAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, 1.0f, 0.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0f, 0.0f, 90.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitYRotationAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      {  0.0f, 1.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   { -1.0f, 0.0f, 0.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0f, 0.0f, 90.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitZRotationAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 0.0f, 1.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0f, 0.0f, 90.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointsOnXYRotatesCorrectly()
    {
        constexpr ScarlettMath::Vec3 pointOne      { -0.1f, -0.5f, 0.0f };
        constexpr ScarlettMath::Vec3 pointTwo      {  0.1f,  0.5f, 0.0f };
        constexpr ScarlettMath::Vec3 solutionOne = {  0.282843f,-0.424264f, 0.0f };
        constexpr ScarlettMath::Vec3 solutionTwo = { -0.282843f, 0.424264f, 0.0f };

        const ScarlettMath::Vec3 resultOne = RotatePoint(pointOne, 0.0f, 0.0f, 45.0f);
        const ScarlettMath::Vec3 resultTwo = RotatePoint(pointTwo, 0.0f, 0.0f, 45.0f);

        return ScarlettMath::IsEqualTo(resultOne, solutionOne) && ScarlettMath::IsEqualTo(resultTwo, solutionTwo);
    }

    inline static bool PointOneTwoThreeAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 1.0f, -0.707107f,3.535534f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0f, 45.0f, 0.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 2.828427f,2.0f,1.414214f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 45.0f, 0.0f, 0.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { -0.707107f,2.121320f,3.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0f, 0.0f, 45.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutXY()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 3.20711f, -0.707107f, 1.79289f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 45.0f, 45.0f, 0.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutYZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.585786f,3.414214f,1.414214f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 45.0f, 0.0f, 45.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutXZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 1.207107f,0.207107f,3.535534f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0f, 45.0f, 45.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutXYZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 2.76777f, 1.76777f, 1.79289f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 45.0f, 45.0f, 45.0f);

        return ScarlettMath::IsEqualTo(result, solution);
    }
};
