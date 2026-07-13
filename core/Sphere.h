#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"

class Sphere : public Object {
public:
    // radius: 球半径
    // segments: 经线分段数（默认36，越多越圆）
    // rings: 纬线圈数（默认18，不含两极）
    explicit Sphere(float radius, int segments = 36, int rings = 18);
};

#endif // SPHERE_H
