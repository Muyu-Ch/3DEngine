#include "Ground.h"

Ground::Ground(float size, float step) {
    std::vector<Vector3> gridVerts;
    std::vector<std::pair<int, int>> gridEdges;

    // 平行于 X 轴的线（在不同 Z 位置）
    for (float z = -size; z <= size; z += step) {
        int v1 = gridVerts.size(); gridVerts.emplace_back(-size, 0.0f, z, 1.0f);
        int v2 = gridVerts.size(); gridVerts.emplace_back( size, 0.0f, z, 1.0f);
        gridEdges.emplace_back(v1, v2);
    }
    // 平行于 Z 轴的线（在不同 X 位置）
    for (float x = -size; x <= size; x += step) {
        int v1 = gridVerts.size(); gridVerts.emplace_back(x, 0.0f, -size, 1.0f);
        int v2 = gridVerts.size(); gridVerts.emplace_back(x, 0.0f,  size, 1.0f);
        gridEdges.emplace_back(v1, v2);
    }

    setVertices(gridVerts);
    setEdges(gridEdges);
}
