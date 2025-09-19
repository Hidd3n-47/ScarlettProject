#pragma once

#include <Math/Quat.h>
#include <Math/Trig.h>

class QuaternionTests
{
public:
    // Todo: It would be better if we removed some of the hardcoded values. The idea was to check that they are correctly generated, however, would be better
    // to compare and see if the rotations (from matrix and from quaternion) result in the same output. This would be more robust and prevent flaky tests.
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
        const ScarlettMath::Quat solution { 0.707107, 0.0,0.0,0.707107 };

        const ScarlettMath::Quat quat {std::numbers::pi * 0.5, 0.0, 0.0 };

        return solution == quat;
    }

    inline static bool CorrectQuaternionForPiOverFourRotationAboutAllAxes()
    {
        const ScarlettMath::Quat solution { 0.844623, 0.191342, 0.46194, 0.191342 };

        constexpr double piOverFour = std::numbers::pi * 0.25;
        const ScarlettMath::Quat quat { piOverFour, piOverFour, piOverFour };

        return solution == quat;
    }

    inline static bool QuaternionReturnsCorrectRotationMatrixWhenNoRotation()
    {
        constexpr ScarlettMath::Mat4 solution = {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };

        const ScarlettMath::Quat quat(0.0, 0.0, 0.0);

        return ScarlettMath::IsEqual(solution, quat.GetRotationMatrix());
    }

    inline static bool QuaternionReturnsCorrectRotationMatrixWhenRotateAboutX()
    {
        constexpr ScarlettMath::Mat4 solution = {
            1.0,  0.0,  0.0, 0.0,
            0.0,  0.0, -1.0, 0.0,
            0.0,  1.0,  0.0, 0.0,
            0.0,  0.0,  0.0, 1.0
        };

        const ScarlettMath::Quat quat{ 0.0, 0.0, std::numbers::pi * 0.5 };

        return ScarlettMath::IsEqual(solution, quat.GetRotationMatrix());
    }

    inline static bool QuaternionReturnsCorrectRotationMatrixWhenRotateAboutY()
    {
        constexpr ScarlettMath::Mat4 solution = {
                  0.5,  0.0, 0.866025, 0.0,
                  0.0,  1.0,      0.0, 0.0,
            -0.866025,  0.0,      0.5, 0.0,
                  0.0,  0.0,      0.0, 1.0
        };

        const ScarlettMath::Quat quat(0.0, std::numbers::pi / 3.0, 0.0);

        return ScarlettMath::IsEqual(solution, quat.GetRotationMatrix());
    }

    inline static bool QuaternionReturnsCorrectRotationMatrixWhenRotateAboutZ()
    {
        constexpr ScarlettMath::Mat4 solution = {
            0.866025,     -0.5, 0.0, 0.0,
                 0.5, 0.866025, 0.0, 0.0,
                 0.0,      0.0, 1.0, 0.0,
                 0.0,      0.0, 0.0, 1.0
        };

        const ScarlettMath::Quat quat{ std::numbers::pi / 6.0, 0.0, 0.0 };

        return ScarlettMath::IsEqual(solution, quat.GetRotationMatrix());
    }

    inline static bool QuaternionReturnsCorrectRotationMatrixAtAnglesOfPiAndTwoPi()
    {
        constexpr ScarlettMath::Mat4 solutionPi = {
             1.0,  0.0,  0.0, 0.0,
             0.0, -1.0,  0.0, 0.0,
             0.0,  0.0, -1.0, 0.0,
             0.0,  0.0,  0.0, 1.0
        };

        constexpr ScarlettMath::Mat4 solutionTwoPi = {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };

        const ScarlettMath::Quat quatPi { 0.0, 0.0, std::numbers::pi };
        const ScarlettMath::Quat quatTwoPi { 0.0, 0.0, 2.0 * std::numbers::pi };

        return ScarlettMath::IsEqual(solutionPi, quatPi.GetRotationMatrix()) &&
               ScarlettMath::IsEqual(solutionTwoPi, quatTwoPi.GetRotationMatrix());
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotation()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Mat4 pointMat   { 1.0, 0.0, 0.0, point.x,
                                                  0.0, 1.0, 0.0, point.y,
                                                  0.0, 0.0, 1.0, point.z,
                                                  0.0, 0.0, 0.0, 1.0 };
        constexpr ScarlettMath::Vec3 solution   { 3.2071f, -0.7072f, 1.7928f };

        const double pitch = ScarlettMath::Radians(45.0);
        const double roll  = ScarlettMath::Radians(45.0);
        const ScarlettMath::Quat rotation { 0.0, pitch, roll };
        const ScarlettMath::Vec3 resultQuat = ScarlettMath::Quat::RotatePoint(point, rotation);
        const ScarlettMath::Mat4 resultMat  = pointMat * rotation.GetRotationMatrix();
        const ScarlettMath::Vec3 resultMatAnswer = { resultMat[0][3], resultMat[1][3], resultMat[2][3] };

        return ScarlettMath::IsEqual(resultQuat, solution) && ScarlettMath::IsEqual(resultMatAnswer, solution);
    }

    inline static ScarlettMath::Mat4 GetRotationMatrix(const double yawDegrees, const double pitchDegrees, const double rollDegrees)
    {
        const double sY = ScarlettMath::Trig::Sin(ScarlettMath::Radians(yawDegrees));
        const double cY = ScarlettMath::Trig::Cos(ScarlettMath::Radians(yawDegrees));
        const double sP = ScarlettMath::Trig::Sin(ScarlettMath::Radians(pitchDegrees));
        const double cP = ScarlettMath::Trig::Cos(ScarlettMath::Radians(pitchDegrees));
        const double sR = ScarlettMath::Trig::Sin(ScarlettMath::Radians(rollDegrees));
        const double cR = ScarlettMath::Trig::Cos(ScarlettMath::Radians(rollDegrees));

        const ScarlettMath::Mat4 rotateX = {
            1.0, 0.0, 0.0, 0.0,
            0.0,  cR, -sR, 0.0,
            0.0,  sR,  cR, 0.0,
            0.0, 0.0, 0.0, 1.0
        };

        const ScarlettMath::Mat4 rotateY = {
              cP, 0.0,   sP, 0.0,
             0.0, 1.0,  0.0, 0.0,
             -sP, 0.0,   cP, 0.0,
             0.0, 0.0,  0.0, 1.0
        };

        const ScarlettMath::Mat4 rotateZ = {
              cY,  -sY, 0.0, 0.0,
              sY,   cY, 0.0, 0.0,
             0.0,  0.0, 1.0, 0.0,
             0.0,  0.0, 0.0, 1.0
        };

        return rotateX * rotateY * rotateZ;
    }

    inline static bool RotationMatrixTest(const double yaw, const double pitch, const double roll)
    {
        const double yawRad     = ScarlettMath::Radians(yaw);
        const double pitchRad   = ScarlettMath::Radians(pitch);
        const double rollRad    = ScarlettMath::Radians(roll);

        const ScarlettMath::Quat rotation { yawRad, pitchRad, rollRad };

        return ScarlettMath::IsEqual(rotation.GetRotationMatrix(), GetRotationMatrix(yaw, pitch, roll));
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest1()
    {
        return RotationMatrixTest(45.0, 45.0, 0.0);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest2()
    {
        return RotationMatrixTest(33.0, 44.0, 55.0);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest3()
    {
        return RotationMatrixTest(90.0, 270.0, 180.0);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest4()
    {
        return RotationMatrixTest(270.0, 90.0, 180.0);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest5()
    {
        return RotationMatrixTest(289.0, 300.0, 100.0);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest6()
    {
        return RotationMatrixTest(3.14, 31.4, 314.1);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest7()
    {
        return RotationMatrixTest(12.3, 123.0, 1.23);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest8()
    {
        return RotationMatrixTest(270.0, 180.0, 90.0);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest9()
    {
        return RotationMatrixTest(90.0, 90.0, 90.0);
    }

    inline static bool RotationMatrixAndQuaternionResultInSameRotationTest10()
    {
        return RotationMatrixTest(56.0, 67.0, 78.0);
    }

    inline static bool AngularEqual(double angle1, double angle2)
    {
        angle1 = ScarlettMath::Trig::CorrectAngleTo0To2PiRange(angle1);
        angle2 = ScarlettMath::Trig::CorrectAngleTo0To2PiRange(angle2);

        return ScarlettMath::IsEqual(angle1, angle2);
    }

    inline static bool YawPitchRollExtractionYieldSameResultOnPointRotation(const double yawDegrees, const double pitchDegrees, const double rollDegrees)
    {
        constexpr ScarlettMath::Vec3 testPoint{123.321f, 961.169f, 47.74f };

        const ScarlettMath::Mat4 rotationMatrix = GetRotationMatrix(yawDegrees, pitchDegrees, rollDegrees);
        const ScarlettMath::Vec3 rotatedPointFromMatrix = ScarlettMath::Vec4{ testPoint, 0.0f } * rotationMatrix;

        const ScarlettMath::Quat quatFromYawPitchRoll{ ScarlettMath::Radians(yawDegrees), ScarlettMath::Radians(pitchDegrees), ScarlettMath::Radians(rollDegrees) };
        const ScarlettMath::Vec3 solutionFromYawPitchRoll = ScarlettMath::Quat::RotatePoint(testPoint, quatFromYawPitchRoll);

        double extractedYaw, extractedPitch, extractedRoll;
        quatFromYawPitchRoll.GetYawPitchRoll(extractedYaw, extractedPitch, extractedRoll);

        const ScarlettMath::Quat quatFromExtractedValues{ extractedYaw, extractedPitch, extractedRoll };
        const ScarlettMath::Vec3 solutionFromExtractedValues = ScarlettMath::Quat::RotatePoint(testPoint, quatFromExtractedValues);

        // Important Note:
        // Quaternions cannot be checked to see if they are equal, there are times when quaternions result in the same rotation, but are different.
        // This happens when gimbal lock occurs. Even though the quaternions aren't equal, they still have the same rotation.
        // For this same reason we cannot check if the rotation matrices are the same.
        // Therefore, the best way to test if quaternion rotations are correct is to check that a rotated point is the same under rotation from matrices and quaternion.
        return  ScarlettMath::IsEqual(rotatedPointFromMatrix, solutionFromYawPitchRoll) &&
                ScarlettMath::IsEqual(solutionFromYawPitchRoll, solutionFromExtractedValues);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionTest1()
    {
        return YawPitchRollExtractionYieldSameResultOnPointRotation(0.0, 0.0, 0.0);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionTest2()
    {
        return YawPitchRollExtractionYieldSameResultOnPointRotation(10.0, 20.0, 30.0);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionTest3()
    {
        return YawPitchRollExtractionYieldSameResultOnPointRotation(45.0, 69.0, 69.42);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionTest4()
    {
        return YawPitchRollExtractionYieldSameResultOnPointRotation(345.0, 369.0, 369.42);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionTest5()
    {
        return YawPitchRollExtractionYieldSameResultOnPointRotation(-345.0, -369.0, -369.42);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionWithGimbalLockTest1()
    {
        return YawPitchRollExtractionYieldSameResultOnPointRotation(90.0, 0.0, 0.0);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionWithGimbalLockTest2()
    {
        return YawPitchRollExtractionYieldSameResultOnPointRotation(90.0, 90.0, 90.0);
    }

    // Todo: Investigate and try and find a fix for this test.
    // This test is known to fail. This is due to not being able to get the correct YPR when gimbal lock occurs.
    // Ignoring this failed test for now as we should try not extract YPR, that is the idea of using Quaternions.
    inline static bool ExtractingYawPitchRollFromQuaternionWithGimbalLockTest3()
    {
        return YawPitchRollExtractionYieldSameResultOnPointRotation(270.0, 90.0, 90.0);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionWithGimbalLockTest4()
    {
        return YawPitchRollExtractionYieldSameResultOnPointRotation(270.0, 32.0, 45.5);
    }

    inline static bool ExtractingYawPitchRollFromQuaternionWithGimbalLockTest5()
    {
        return YawPitchRollExtractionYieldSameResultOnPointRotation(90.0, -369.0, -369.42);
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
        quat.Normalize();

        quat *= quat.Inverse();

        return quat.IsIdentity();
    }

    inline static bool QuaternionMultipliedByQuaternion()
    {
        constexpr double angle1 = std::numbers::pi * 0.5;
        constexpr double angle2 = std::numbers::pi / 3.0;

        const ScarlettMath::Quat quat1 { ScarlettMath::Radians(angle1), 0.0, 0.0 };
        const ScarlettMath::Quat quat2 { ScarlettMath::Radians(angle2), 0.0, 0.0 };

        const ScarlettMath::Quat solution { ScarlettMath::Radians(angle1 + angle2), 0.0, 0.0 };

        const ScarlettMath::Quat answer = quat1 * quat2;

        return solution == answer;
    }

    inline static bool QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest1()
    {
        constexpr double angle1 = std::numbers::pi * 0.5;
        constexpr double angle2 = std::numbers::pi / 3.0;

        const ScarlettMath::Quat quat1 { angle1, 0.0, 0.0 };
        const ScarlettMath::Quat quat2 { angle2, 0.0, 0.0 };

        const ScarlettMath::Quat answer = quat1 * quat2;

        double answerYaw, answerPitch, answerRoll;
        answer.GetYawPitchRoll(answerYaw, answerPitch, answerRoll);

        return YawPitchRollExtractionYieldSameResultOnPointRotation(answerYaw, answerPitch, answerRoll);
    }

    inline static bool QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest2()
    {
        constexpr double angle1 = std::numbers::pi * 0.5;
        constexpr double angle2 = std::numbers::pi / 3.0;

        const ScarlettMath::Quat quat1 { angle1, 0.0, 0.0 };
        const ScarlettMath::Quat quat2 { 0.0, angle2, 0.0 };

        const ScarlettMath::Quat answer = quat1 * quat2;
        double answerYaw, answerPitch, answerRoll;
        answer.GetYawPitchRoll(answerYaw, answerPitch, answerRoll);

        return YawPitchRollExtractionYieldSameResultOnPointRotation(answerYaw, answerPitch, answerRoll);
    }

    inline static bool QuaternionMultipliedByQuaternionHasCorrectYawPitchRollTest3()
    {
        constexpr double angle1 = ScarlettMath::Radians(11.9);
        constexpr double angle2 = ScarlettMath::Radians(22.8);
        constexpr double angle3 = ScarlettMath::Radians(33.7);
        constexpr double angle4 = ScarlettMath::Radians(44.6);
        constexpr double angle5 = ScarlettMath::Radians(55.5);
        constexpr double angle6 = ScarlettMath::Radians(66.4);

        const ScarlettMath::Quat quat1 { angle1, angle2, angle3 };
        const ScarlettMath::Quat quat2 { angle4, angle5, angle6 };

        const ScarlettMath::Quat answer = quat1 * quat2;

        double answerYaw, answerPitch, answerRoll;
        answer.GetYawPitchRoll(answerYaw, answerPitch, answerRoll);

        return YawPitchRollExtractionYieldSameResultOnPointRotation(answerYaw, answerPitch, answerRoll);
    }

    inline static bool OriginDoesNotRotate()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ ScarlettMath::Radians(20.0f), ScarlettMath::Radians(30.0f), ScarlettMath::Radians(40.0f) });

        return ScarlettMath::IsEqual(result, solution);
    }

    static ScarlettMath::Vec3 RotatePoint(const ScarlettMath::Vec3 point, const double yawDeg, const double pitchDeg, const double rollDeg)
    {
        const double yaw   = ScarlettMath::Radians(yawDeg);
        const double pitch = ScarlettMath::Radians(pitchDeg);
        const double roll  = ScarlettMath::Radians(rollDeg);
        return ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ yaw, pitch, roll });
    }

    inline static bool PointRotateAndUnRotate()
    {
        constexpr ScarlettMath::Vec3 point      { 1.2f, 2.4f, 3.6f };

        constexpr double yaw   = ScarlettMath::Radians(15.0);
        constexpr double pitch = ScarlettMath::Radians(25.0);
        constexpr double roll  = ScarlettMath::Radians(35.0);
        const ScarlettMath::Quat rotation = ScarlettMath::Quat{ yaw, pitch, roll };

        const ScarlettMath::Vec3 rotate = ScarlettMath::Quat::RotatePoint(point, rotation);
        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(rotate, rotation.Inverse());

        return ScarlettMath::IsEqual(result, point);
    }

    inline static bool UnitXRotationAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0, 0.0, 90.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool UnitYRotationAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 1.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, 0.0f, 1.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0, 0.0, 90.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool UnitZRotationAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f,  0.0f, 1.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, -1.0f, 0.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0, 0.0, 90.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool UnitXRotationAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 0.0f,  0.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, 0.0f, -1.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0, 90.0, 0.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool UnitYRotationAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 1.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0, 90.0, 0.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool UnitZRotationAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 0.0f, 1.0f };
        constexpr ScarlettMath::Vec3 solution   { 1.0f, 0.0f, 0.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0, 90.0, 0.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool UnitXRotationAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, 1.0f, 0.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 90.0, 0.0, 0.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool UnitYRotationAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      {  0.0f, 1.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   { -1.0f, 0.0f, 0.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 90.0, 0.0, 0.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool UnitZRotationAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 0.0f, 1.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = RotatePoint(point, 90.0, 0.0, 0.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool PointsOnXYRotatesCorrectly()
    {
        constexpr ScarlettMath::Vec3 pointOne      { -0.1f, -0.5f, 0.0f };
        constexpr ScarlettMath::Vec3 pointTwo      {  0.1f,  0.5f, 0.0f };
        constexpr ScarlettMath::Vec3 solutionOne = {  0.282843f,-0.424264f, 0.0f };
        constexpr ScarlettMath::Vec3 solutionTwo = { -0.282843f, 0.424264f, 0.0f };

        const ScarlettMath::Vec3 resultOne = RotatePoint(pointOne, 45.0, 0.0, 0.0);
        const ScarlettMath::Vec3 resultTwo = RotatePoint(pointTwo, 45.0, 0.0, 0.0);

        return ScarlettMath::IsEqual(resultOne, solutionOne) && ScarlettMath::IsEqual(resultTwo, solutionTwo);
    }

    inline static bool PointOneTwoThreeAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 1.0f, -0.707107f,3.535534f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0, 0.0, 45.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool PointOneTwoThreeAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 2.828427f,2.0f,1.414214f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0, 45.0, 0.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool PointOneTwoThreeAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { -0.707107f,2.121320f,3.0f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 45.0, 0.0, 0.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool PointOneTwoThreeAboutXY()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 3.20711f, -0.707107f, 1.79289f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 0.0, 45.0, 45.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool PointOneTwoThreeAboutYZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.585786f,3.414214f,1.414214f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 45.0, 45.0, 0.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool PointOneTwoThreeAboutXZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 1.207107f,0.207107f,3.535534f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 45.0, 0.0, 45.0);

        return ScarlettMath::IsEqual(result, solution);
    }

    inline static bool PointOneTwoThreeAboutXYZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 2.76777f, 1.76777f, 1.79289f };

        const ScarlettMath::Vec3 result = RotatePoint(point, 45.0, 45.0, 45.0);

        return ScarlettMath::IsEqual(result, solution);
    }
};
