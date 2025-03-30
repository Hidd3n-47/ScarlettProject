#pragma once

#include <Math/Math.h>

class BoundingBoxIntersectionTests
{
public:
    explicit inline BoundingBoxIntersectionTests(Scarlett::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionOnXAxis", BoundingBoxIntersectionOnXAxis);
    }

    inline static bool BoundingBoxIntersectionOnXAxis()
    {
        bool passed = true;

        ScarlettMath::BoundingBox box;

        return passed;
    }

};
