#pragma once

#include <Math/Quat.h>
#include <Math/Trig.h>

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
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotationTest1", RotationMatrixAndQuaternionResultInSameRotationTest1);
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotationTest2", RotationMatrixAndQuaternionResultInSameRotationTest2);
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotationTest3", RotationMatrixAndQuaternionResultInSameRotationTest3);
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotationTest4", RotationMatrixAndQuaternionResultInSameRotationTest4);
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotationTest5", RotationMatrixAndQuaternionResultInSameRotationTest5);
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotationTest6", RotationMatrixAndQuaternionResultInSameRotationTest6);
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotationTest7", RotationMatrixAndQuaternionResultInSameRotationTest7);
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotationTest8", RotationMatrixAndQuaternionResultInSameRotationTest8);
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotationTest9", RotationMatrixAndQuaternionResultInSameRotationTest9);
        testRegistry->AddTestCase("Quaternion Tests", "RotationMatrixAndQuaternionResultInSameRotationTest10", RotationMatrixAndQuaternionResultInSameRotationTest10);

        // Extracting Yaw, Pitch and Roll from Quaternion.
        testRegistry->AddTestCase("Quaternion Tests", "ExtractingYawPitchRollFromQuaternionTest1", ExtractingYawPitchRollFromQuaternionTest1);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractingYawPitchRollFromQuaternionTest2", ExtractingYawPitchRollFromQuaternionTest2);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractingYawPitchRollFromQuaternionTest3", ExtractingYawPitchRollFromQuaternionTest3);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractingYawPitchRollFromQuaternionTest4", ExtractingYawPitchRollFromQuaternionTest4);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractingYawPitchRollFromQuaternionTest5", ExtractingYawPitchRollFromQuaternionTest5);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractingYawPitchRollFromQuaternionWithGimbalLockTest1", ExtractingYawPitchRollFromQuaternionWithGimbalLockTest1);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractingYawPitchRollFromQuaternionWithGimbalLockTest2", ExtractingYawPitchRollFromQuaternionWithGimbalLockTest2);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractingYawPitchRollFromQuaternionWithGimbalLockTest3", ExtractingYawPitchRollFromQuaternionWithGimbalLockTest3);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractingYawPitchRollFromQuaternionWithGimbalLockTest4", ExtractingYawPitchRollFromQuaternionWithGimbalLockTest4);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractingYawPitchRollFromQuaternionWithGimbalLockTest5", ExtractingYawPitchRollFromQuaternionWithGimbalLockTest5);

        // Quaternion multiplication.
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionMultipliedByInverseIsIdentity", QuaternionMultipliedByInverseIsIdentity);
        testRegistry->AddTestCase("Quaternion Tests", "ComplexQuaternionMultipliedByInverseIsIdentity", ComplexQuaternionMultipliedByInverseIsIdentity);
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionMultipliedByQuaternion", QuaternionMultipliedByQuaternion);

        // Quaternion multiplication correct Yaw, Pitch and Roll.
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest1", QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest1);
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest2", QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest2);
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest3", QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest3);

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

        const ScarlettMath::Quat quat { 0.0f, 0.0f, std::numbers::pi_v<float> * 0.5f };

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

        const ScarlettMath::Quat quat(0.0f, 0.0f, 0.0f);

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

        const ScarlettMath::Quat quat(0.0f, std::numbers::pi_v<float> / 3.0f, 0.0f);

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

        const ScarlettMath::Quat quat { std::numbers::pi_v<float> / 6.0f, 0.0f, 0.0f };

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

        const ScarlettMath::Quat quat {0.0f, 0.0f, std::numbers::pi_v<float> * 0.5f };

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

        const ScarlettMath::Quat quatPi { 0.0f, 0.0f, std::numbers::pi_v<float> };
        const ScarlettMath::Quat quatTwoPi { 0.0f, 0.0f, 2.0f * std::numbers::pi_v<float> };

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

    inline static ScarlettMath::Mat4 GetRotationMatrix(const float yaw, const float pitch, const float roll)
    {
        const float sY = ScarlettMath::Trig::Sin(ScarlettMath::Radians(yaw));
        const float cY = ScarlettMath::Trig::Cos(ScarlettMath::Radians(yaw));
        const float sP = ScarlettMath::Trig::Sin(ScarlettMath::Radians(pitch));
        const float cP = ScarlettMath::Trig::Cos(ScarlettMath::Radians(pitch));
        const float sR = ScarlettMath::Trig::Sin(ScarlettMath::Radians(roll));
        const float cR = ScarlettMath::Trig::Cos(ScarlettMath::Radians(roll));

        const ScarlettMath::Mat4 rotateX = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,   cP,  -sP, 0.0f,
            0.0f,   sP,   cP, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        const ScarlettMath::Mat4 rotateY = {
              cY, 0.0f,   sY, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
             -sY, 0.0f,   cY, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        const ScarlettMath::Mat4 rotateZ = {
              cR,  -sR, 0.0f, 0.0f,
              sR,   cR, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        return rotateX * rotateY * rotateZ;
    }

    inline static bool RotationMatrixTest(const float yaw, const float pitch, const float roll)
    {
        const float yawRad     = ScarlettMath::Radians(yaw);
        const float pitchRad   = ScarlettMath::Radians(pitch);
        const float rollRad    = ScarlettMath::Radians(roll);

        const ScarlettMath::Quat rotation { yawRad, pitchRad, rollRad };

        return ScarlettMath::IsEqualTo(rotation.GetRotationMatrix(), GetRotationMatrix(yaw, pitch, roll));
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest1()
    {
        return RotationMatrixTest(45.0f, 45.0f, 0.0f);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest2()
    {
        return RotationMatrixTest(33.0f, 44.0f, 55.0f);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest3()
    {
        return RotationMatrixTest(90.0f, 270.0f, 180.0f);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest4()
    {
        return RotationMatrixTest(270.0f, 90.0f, 180.0f);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest5()
    {
        return RotationMatrixTest(289.0f, 300.0f, 100.0f);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest6()
    {
        return RotationMatrixTest(3.14f, 31.4f, 314.1f);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest7()
    {
        return RotationMatrixTest(12.3f, 123.0f, 1.23f);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest8()
    {
        return RotationMatrixTest(270.0f, 180.0f, 90.0f);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest9()
    {
        return RotationMatrixTest(90.0f, 90.0f, 90.0f);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest10()
    {
        return RotationMatrixTest(56.0f, 67.0f, 78.0f);
    }

    inline static bool AngularEqual(float angle1, float angle2)
    {
        angle1 = ScarlettMath::Trig::CorrectAngleTo0To2PiRange(angle1);
        angle2 = ScarlettMath::Trig::CorrectAngleTo0To2PiRange(angle2);

        return ScarlettMath::IsEqualTo(angle1, angle2);
    }

    inline static bool YawPitchRollExtractionTest(const float yaw, const float pitch, const float roll)
    {
        const float yawSolution   = ScarlettMath::Radians(yaw);
        const float pitchSolution = ScarlettMath::Radians(pitch);
        const float rollSolution  = ScarlettMath::Radians(roll);

        const ScarlettMath::Quat quat { yawSolution, pitchSolution, rollSolution };

        float yawExtracted, pitchExtracted, rollExtracted;
        quat.GetYawPitchRoll(yawExtracted, pitchExtracted, rollExtracted);

        return AngularEqual(yawExtracted, yawSolution) && AngularEqual(pitchExtracted, pitchSolution) && AngularEqual(rollExtracted, rollSolution);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionTest1()
    {
        return YawPitchRollExtractionTest(0.0f, 0.0f, 0.0f);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionTest2()
    {
        return YawPitchRollExtractionTest(10.0f, 20.0f, 30.0f);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionTest3()
    {
        return YawPitchRollExtractionTest(45.0f, 69.0f, 69.420f);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionTest4()
    {
        return YawPitchRollExtractionTest(345.0f, 369.0f, 369.420f);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionTest5()
    {
        return YawPitchRollExtractionTest(-345.0f, -369.0f, -369.420f);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionWithGimbalLockTest1()
    {
        return YawPitchRollExtractionTest(90.0f, 0.0f, 0.0f);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionWithGimbalLockTest2()
    {
        return YawPitchRollExtractionTest(90.0f, 90.0f, 90.0f);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionWithGimbalLockTest3()
    {
        return YawPitchRollExtractionTest(270.0f, 90.0f, 90.0f);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionWithGimbalLockTest4()
    {
        return YawPitchRollExtractionTest(270.0f, 32.0f, 45.5f);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionWithGimbalLockTest5()
    {
        return YawPitchRollExtractionTest(90.0f, -369.0f, -369.420f);
    }

    inline static bool QuaternionMultipliedByInverseIsIdentity()
    {
        ScarlettMath::Quat quat { ScarlettMath::Radians(45.0f), 0.0f, 0.0f };
        quat *= quat.Inverse();

        return quat.IsIdentity();
    }

    inline static bool ComplexQuaternionMultipliedByInverseIsIdentity()
    {
        ScarlettMath::Quat quat { ScarlettMath::Radians(22.0f), ScarlettMath::Radians(33.0f), ScarlettMath::Radians(44.0f) };
        quat.Normalise();

        quat *= quat.Inverse();

        return quat.IsIdentity();
    }

    inline static bool QuaternionMultipliedByQuaternion()
    {
        constexpr float angle1 = std::numbers::pi_v<float> * 0.5f;
        constexpr float angle2 = std::numbers::pi_v<float> / 3.0f;

        const ScarlettMath::Quat quat1 { ScarlettMath::Radians(angle1), 0.0f, 0.0f };
        const ScarlettMath::Quat quat2 { ScarlettMath::Radians(angle2), 0.0f, 0.0f };

        const ScarlettMath::Quat solution { ScarlettMath::Radians(angle1 + angle2), 0.0f, 0.0f };

        const ScarlettMath::Quat answer = quat1 * quat2;

        return solution == answer;
    }

    inline static bool QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest1()
    {
        constexpr float angle1 = std::numbers::pi_v<float> * 0.5f;
        constexpr float angle2 = std::numbers::pi_v<float> / 3.0f;

        const ScarlettMath::Quat quat1 { angle1, 0.0f, 0.0f };
        const ScarlettMath::Quat quat2 { angle2, 0.0f, 0.0f };

        const ScarlettMath::Quat answer = quat1 * quat2;

        float answerYaw, answerPitch, answerRoll;
        answer.GetYawPitchRoll(answerYaw, answerPitch, answerRoll);

        return AngularEqual(answerYaw, angle1 + angle2) && AngularEqual(answerPitch, 0.0f) && AngularEqual(answerRoll, 0.0f);
    }

    inline static bool QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest2()
    {
        constexpr float angle1 = std::numbers::pi_v<float> * 0.5f;
        constexpr float angle2 = std::numbers::pi_v<float> / 3.0f;

        const ScarlettMath::Quat quat1 { angle1, 0.0f, 0.0f };
        const ScarlettMath::Quat quat2 { 0.0f, angle2, 0.0f };

        const ScarlettMath::Quat answer = quat1 * quat2;
        float answerYaw, answerPitch, answerRoll;
        answer.GetYawPitchRoll(answerYaw, answerPitch, answerRoll);

        return AngularEqual(angle1, answerYaw) && AngularEqual(answerPitch, angle2) && AngularEqual(answerRoll, 0.0f);
    }

    inline static bool QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest3()
    {
        const float angle1 = ScarlettMath::Radians(11.9f);
        const float angle2 = ScarlettMath::Radians(22.8f);
        const float angle3 = ScarlettMath::Radians(33.7f);
        const float angle4 = ScarlettMath::Radians(44.6f);
        const float angle5 = ScarlettMath::Radians(55.5f);
        const float angle6 = ScarlettMath::Radians(66.4f);

        const ScarlettMath::Quat quat1 { angle1, angle2, angle3 };
        const ScarlettMath::Quat quat2 { angle4, angle5, angle6 };

        const ScarlettMath::Quat answer = quat1 * quat2;

        float answerYaw, answerPitch, answerRoll;
        answer.GetYawPitchRoll(answerYaw, answerPitch, answerRoll);

        return AngularEqual(answerYaw, angle1 + angle4) && AngularEqual(answerPitch, angle2 + angle5) && AngularEqual(answerRoll, angle3 + angle6);
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
