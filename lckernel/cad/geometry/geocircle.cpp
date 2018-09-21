#include "geocircle.h"

using namespace lc;
using namespace geo;

Circle::Circle(Coordinate center, double radius) :
        Base(),
        _center(std::move(center)),
        _radius(radius) {
    if (radius < 0.0) {
        throw std::runtime_error("Invalid radius");
    }

}

const Coordinate Circle::center() const {
    return _center;
}
double Circle::radius() const {
    return _radius;
}


Coordinate Circle::nearestPointOnPath(const Coordinate& coord) const {
    return nearestPointOnEntity(coord);
}
Coordinate Circle::nearestPointOnEntity(const Coordinate& coord) const {
    return _center + Coordinate((coord - _center).angle()) * _radius;
}