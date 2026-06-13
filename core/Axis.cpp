#include "Axis.h"

Axis::Axis(float length) {
    setVertices({
        Vector3(0, 0, 0, 1),          // 0: 原点
        Vector3(length, 0, 0, 1),     // 1: X 方向端点
        Vector3(0, length, 0, 1),     // 2: Y 方向端点
        Vector3(0, 0, length, 1)      // 3: Z 方向端点
    });
    setEdges({
        {0, 1},  // X 轴 → 红色
        {0, 2},  // Y 轴 → 绿色
        {0, 3}   // Z 轴 → 蓝色
    });
}
