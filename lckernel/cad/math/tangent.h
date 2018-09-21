#pragma once

#include <vector>
#include <cad/geometry/geocircle.h>

namespace lc {
    namespace math {
        class Tangent {
            public:
                static std::vector<geo::Coordinate> lineTangent(const geo::Circle& circle, const geo::Coordinate& extPoint);
        };
    }
}