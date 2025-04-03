#pragma once

#include <Math/Ray.h>

class BoundingBoxIntersectionTests
{
public:
    explicit inline BoundingBoxIntersectionTests(Scarlett::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionOnXAxis", BoundingBoxIntersectionOnXAxis);
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionFails", BoundingBoxIntersectionFails);
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionOnTransformedBox", BoundingBoxIntersectionOnTransformedBox);
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionOnTranslatedAndRotatedBox", BoundingBoxIntersectionOnTranslatedAndRotatedBox);
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionOnRotatedBox", BoundingBoxIntersectionOnRotatedBox);
    }

    inline static bool BoundingBoxIntersectionOnXAxis()
    {
        const ScarlettMath::Ray ray { { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f} };
        const ScarlettMath::BoundingBox box;

        return ScarlettMath::Ray::RayIntersectsWithBoundingBox(ray, box);
    }

    inline static bool BoundingBoxIntersectionFails()
    {
        const ScarlettMath::Ray ray { { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f} };
        const ScarlettMath::BoundingBox box;

        return !ScarlettMath::Ray::RayIntersectsWithBoundingBox(ray, box);
    }

    inline static bool BoundingBoxIntersectionOnTransformedBox()
    {
        const ScarlettMath::Ray ray { { 1.0f, 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f} };
        ScarlettMath::BoundingBox box;
        box.SetTranslation({0.0f, 1.0f, 0.0f});

        return ScarlettMath::Ray::RayIntersectsWithBoundingBox(ray, box);
    }

    inline static bool BoundingBoxIntersectionOnTranslatedAndRotatedBox()
    {
        const ScarlettMath::Ray ray { { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f} };

        ScarlettMath::BoundingBox box { { 0.0f, -1.0f, -1.0f}, { 0.5f, 1.0f, 1.0f} };
        box.SetTranslation({1.0f, 0.0f, 0.0f });
        box.SetRotation(ScarlettMath::Quat{ ScarlettMath::Radians(90.f), 0.0f, 0.0f });

        return ScarlettMath::Ray::RayIntersectsWithBoundingBox(ray, box);
    }

    inline static bool BoundingBoxIntersectionOnRotatedBox()
    {
        const ScarlettMath::Ray rayX { { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f} };
        const ScarlettMath::Ray rayY { { -0.3f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f} };

        ScarlettMath::BoundingBox box;
        box.SetTranslation({0.5f, 0.5f, 0.0f});
        box.SetRotation(ScarlettMath::Quat{ ScarlettMath::Radians(45.f), 0.0f, 0.0f });

        return ScarlettMath::Ray::RayIntersectsWithBoundingBox(rayX, box) && ScarlettMath::Ray::RayIntersectsWithBoundingBox(rayY, box);
    }
};
