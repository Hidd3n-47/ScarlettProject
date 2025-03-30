#pragma once

#include <Math/Math.h>

class QuaternionTests
{
public:
    explicit inline QuaternionTests(Scarlett::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("Quaternion Tests", "OriginDoesNotRotate", OriginDoesNotRotate);
        testRegistry->AddTestCase("Quaternion Tests", "UnitXRotationAboutX", UnitXRotationAboutX);
        testRegistry->AddTestCase("Quaternion Tests", "UnitYRotationAboutX", UnitYRotationAboutX);
        testRegistry->AddTestCase("Quaternion Tests", "UnitZRotationAboutX", UnitZRotationAboutX);
        testRegistry->AddTestCase("Quaternion Tests", "UnitXRotationAboutY", UnitXRotationAboutY);
        testRegistry->AddTestCase("Quaternion Tests", "UnitYRotationAboutY", UnitYRotationAboutY);
        testRegistry->AddTestCase("Quaternion Tests", "UnitZRotationAboutY", UnitZRotationAboutY);
        testRegistry->AddTestCase("Quaternion Tests", "UnitXRotationAboutZ", UnitXRotationAboutZ);
        testRegistry->AddTestCase("Quaternion Tests", "UnitYRotationAboutZ", UnitYRotationAboutZ);
        testRegistry->AddTestCase("Quaternion Tests", "UnitZRotationAboutZ", UnitZRotationAboutZ);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutX", PointOneTwoThreeAboutX);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutY", PointOneTwoThreeAboutY);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutZ", PointOneTwoThreeAboutZ);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutXY", PointOneTwoThreeAboutXY);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutYZ", PointOneTwoThreeAboutYZ);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutXZ", PointOneTwoThreeAboutXZ);
        testRegistry->AddTestCase("Quaternion Tests", "PointOneTwoThreeAboutXYZ", PointOneTwoThreeAboutXYZ);
    }

    inline static bool OriginDoesNotRotate()
    {
        constexpr ScarlettMath::Vec3 origin     { 0.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   = origin;

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(origin, ScarlettMath::Quat{ 20.0f, 30.0f, 40.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitXRotationAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 0.0f, 90.0f, 0.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitYRotationAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 1.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, 0.0f, 1.0f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 0.0f, 90.0f, 0.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitZRotationAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 0.0f, 1.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, -1.0f, 0.0f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 0.0f, 90.0f, 0.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitXRotationAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, 0.0f, -1.0f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 90.0f, 0.0f, 0.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitYRotationAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 1.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 90.0f, 0.0f, 0.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitZRotationAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 0.0f, 1.0f };
        constexpr ScarlettMath::Vec3 solution   { 1.0f, 0.0f, 0.0f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 90.0f, 0.0f, 0.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitXRotationAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 0.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.0f, 1.0f, 0.0f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 0.0f, 0.0f, 90.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitYRotationAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 1.0f, 0.0f };
        constexpr ScarlettMath::Vec3 solution   { -1.0f, 0.0f, 0.0f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 0.0f, 0.0f, 90.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool UnitZRotationAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      { 0.0f, 0.0f, 1.0f };
        constexpr ScarlettMath::Vec3 solution   = point;

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 0.0f, 0.0f, 90.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutX()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 1.0f, -0.707107f,3.535534f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 0.0f, 45.0f, 0.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutY()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 2.828427f,2.0f,1.414214f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 45.0f, 0.0f, 0.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { -0.707107f,2.121320f,3.0f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 0.0f, 0.0f, 45.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutXY()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 2.828427f,0.414214f,2.414214f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 45.0f, 45.0f, 0.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutYZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.585786f,3.414214f,1.414214f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 45.0f, 0.0f, 45.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutXZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 1.207107f,0.207107f,3.535534f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 0.0f, 45.0f, 45.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }

    inline static bool PointOneTwoThreeAboutXYZ()
    {
        constexpr ScarlettMath::Vec3 point      { 1.0f, 2.0f, 3.0f };
        constexpr ScarlettMath::Vec3 solution   { 0.585786f,3.414214f,1.414214f };

        const ScarlettMath::Vec3 result = ScarlettMath::Quat::RotatePoint(point, ScarlettMath::Quat{ 45.0f, 45.0f, 45.0f });

        return ScarlettMath::IsEqualTo(result, solution);
    }
};
