#ifndef BALL_H
#define BALL_H

#include "Shape.h"

class Ball : public Shape {
public:
    // radius: 球半径
    // segments: 经线分段数（默认36，越多越圆）
    // rings: 纬线圈数（默认18，不含两极）
    explicit Ball(float radius, int segments = 36, int rings = 18);
};

#endif // BALL_H
