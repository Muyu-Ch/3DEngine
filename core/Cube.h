#ifndef CUBE_H
#define CUBE_H

#include "Object.h"

class Cube : public Object {
public:
    // halfX, halfY: X/Y 方向半边长，zNear, zFar: Z 方向前后平面
    Cube(float halfX, float halfY, float zNear, float zFar);
};

#endif // CUBE_H
