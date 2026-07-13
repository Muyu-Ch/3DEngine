#include "Object.h"

Object::Object() {
    modelMatrix = Matrix4();  // 初始为单位矩阵
}

void Object::setVertices(const std::vector<Vector3>& v) {
    localVertices = v;
}

void Object::setEdges(const std::vector<std::pair<int, int>>& e) {
    edgeIndices = e;
}

void Object::setAllPairEdges() {
    edgeIndices.clear();
    for (size_t i = 0; i < localVertices.size(); i++) {
        for (size_t j = i + 1; j < localVertices.size(); j++) {
            edgeIndices.emplace_back(i, j);
        }
    }
}

std::vector<std::pair<Vector3, Vector3>> Object::getRenderEdges(const Matrix4& viewMatrix) const {
    // 合并 model 和 view 矩阵：先 model 后 view
    Matrix4 mvp = viewMatrix * modelMatrix;

    // 变换所有顶点到 view space
    std::vector<Vector3> transformed;
    transformed.reserve(localVertices.size());
    for (const auto& v : localVertices) {
        transformed.push_back(mvp.MultiplyVector(v));
    }

    // 用索引组装线段
    std::vector<std::pair<Vector3, Vector3>> result;
    result.reserve(edgeIndices.size());
    for (const auto& e : edgeIndices) {
        result.emplace_back(transformed[e.first], transformed[e.second]);
    }

    return result;
}

Vector3 Object::getCenter() const {
    Vector3 center;
    center.center(localVertices);
    return center;
}
