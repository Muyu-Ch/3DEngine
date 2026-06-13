#ifndef AXIS_H
#define AXIS_H

#include "Objects.h"

class Axis : public Objects {
public:
    // length: 坐标轴长度
    Axis(float length);

    // 索引约定：edge 0 = X轴(红), edge 1 = Y轴(绿), edge 2 = Z轴(蓝)
    enum { X_EDGE = 0, Y_EDGE = 1, Z_EDGE = 2 };
};

#endif // AXIS_H
