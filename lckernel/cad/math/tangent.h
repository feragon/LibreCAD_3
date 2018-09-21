#pragma once

#include <vector>
#include <cad/geometry/geocircle.h>

namespace lc {
    namespace math {
        class Tangent {
            public:
                //TODO: Would be better using geo::Circle class. Unable to to this because of Lua
                static std::vector<geo::Coordinate> lineTangent(const entity::Circle_CSPtr& circle, const geo::Coordinate& extPoint);
        };
    }
}