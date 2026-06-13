#include "Shape.h"

Shape::Shape() : Objects() {}

Vector3 Shape::getCenter() const {
    Vector3 center;
    center.center(localVertices);
    return center;
}
