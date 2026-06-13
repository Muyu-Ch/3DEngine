#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape {
public:
    // halfX, halfY: X/Y 方向半边长，zNear, zFar: Z 方向前后平面
    Cube(float halfX, float halfY, float zNear, float zFar);
};

#endif // CUBE_H
