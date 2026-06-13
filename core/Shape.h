#ifndef SHAPE_H
#define SHAPE_H

#include "Objects.h"

class Shape : public Objects {
public:
    Shape();

    // 计算 localVertices 的几何中心
    Vector3 getCenter() const;
};

#endif // SHAPE_H
