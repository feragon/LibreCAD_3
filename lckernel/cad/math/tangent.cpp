#include "tangent.h"
#include <cad/functions/intersect.h>

using namespace lc;
using namespace math;

std::vector<geo::Coordinate> Tangent::lineTangent(const entity::Circle_CSPtr& circle, const geo::Coordinate& extPoint) {
    double distance = extPoint.distanceTo(circle->center());
    double tangentLength = sqrt(distance * distance - circle->radius() * circle->radius());

    geo::Circle intersectionCircle(extPoint, tangentLength);
    Intersect intersect(Intersect::OnEntity, LCTOLERANCE);
    intersect(*circle, intersectionCircle);
    return intersect.result();
}
