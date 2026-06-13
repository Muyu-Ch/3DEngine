#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <utility>
#include "Vector3.h"
#include "Matrix4.h"

class Objects {
public:
    std::vector<Vector3> localVertices;           // 物体本地空间顶点
    std::vector<std::pair<int, int>> edgeIndices; // 线段：顶点索引对
    Matrix4 modelMatrix;                           // 模型变换矩阵

    Objects();

    // 设置顶点
    void setVertices(const std::vector<Vector3>& v);
    // 设置线段（顶点索引对）
    void setEdges(const std::vector<std::pair<int, int>>& e);
    // 快捷方法：所有顶点两两连线
    void setAllPairEdges();

    // 获取经过 modelMatrix * viewMatrix 变换后的线段（可直接用于渲染）
    std::vector<std::pair<Vector3, Vector3>> getRenderEdges(const Matrix4& viewMatrix) const;
};

#endif // OBJECTS_H
