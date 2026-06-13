#include "Football.h"
#include <cmath>

Football::Football(float radius) {
    const float phi = (1.0f + std::sqrt(5.0f)) / 2.0f;

    // 第一步：生成截角二十面体的 60 个顶点（三组坐标 + 循环排列 + 正负号组合）
    std::vector<Vector3> rawVerts;

    // 辅助：添加 (x,y,z), (z,x,y), (y,z,x) 三个循环排列
    auto addCyclic = [&](float x, float y, float z) {
        rawVerts.emplace_back(x, y, z, 1.0f);
        rawVerts.emplace_back(z, x, y, 1.0f);
        rawVerts.emplace_back(y, z, x, 1.0f);
    };

    // 组 1: (0, ±1, ±3φ) → 12 个顶点
    {
        float a = 1.0f, b = 3.0f * phi;
        addCyclic( 0,  a,  b);
        addCyclic( 0,  a, -b);
        addCyclic( 0, -a,  b);
        addCyclic( 0, -a, -b);
    }

    // 组 2: (±1, ±(2+φ), ±2φ) → 24 个顶点
    {
        float a = 1.0f, b = 2.0f + phi, c = 2.0f * phi;
        for (int si = 0; si < 2; si++)
            for (int sj = 0; sj < 2; sj++)
                for (int sk = 0; sk < 2; sk++)
                    addCyclic(si ? a : -a, sj ? b : -b, sk ? c : -c);
    }

    // 组 3: (±φ, ±2, ±(2φ+1)) → 24 个顶点
    {
        float a = phi, b = 2.0f, c = 2.0f * phi + 1.0f;
        for (int si = 0; si < 2; si++)
            for (int sj = 0; sj < 2; sj++)
                for (int sk = 0; sk < 2; sk++)
                    addCyclic(si ? a : -a, sj ? b : -b, sk ? c : -c);
    }

    // 去重（理论上 60 个均唯一，浮点安全兜底）
    std::vector<Vector3> unique;
    const float dupEps = 0.001f;
    for (const auto& v : rawVerts) {
        bool dup = false;
        for (const auto& u : unique) {
            float dx = v.x - u.x, dy = v.y - u.y, dz = v.z - u.z;
            if (std::sqrt(dx*dx + dy*dy + dz*dz) < dupEps) { dup = true; break; }
        }
        if (!dup) unique.push_back(v);
    }

    // 归一化到目标半径（截角二十面体顶点共球面）
    localVertices.clear();
    for (const auto& v : unique) {
        float len = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
        localVertices.emplace_back(
            v.x / len * radius,
            v.y / len * radius,
            v.z / len * radius,
            1.0f
        );
    }

    // 第二步：找出棱长 = 所有顶点对之间的最短距离
    float edgeLen = 1e10f;
    for (size_t i = 0; i < localVertices.size(); i++) {
        for (size_t j = i + 1; j < localVertices.size(); j++) {
            float dx = localVertices[i].x - localVertices[j].x;
            float dy = localVertices[i].y - localVertices[j].y;
            float dz = localVertices[i].z - localVertices[j].z;
            float d = std::sqrt(dx*dx + dy*dy + dz*dz);
            if (d < edgeLen) edgeLen = d;
        }
    }

    // 第三步：所有距离 == 棱长的点对即为边（容差 1%）
    edgeIndices.clear();
    float tol = edgeLen * 0.01f;
    for (size_t i = 0; i < localVertices.size(); i++) {
        for (size_t j = i + 1; j < localVertices.size(); j++) {
            float dx = localVertices[i].x - localVertices[j].x;
            float dy = localVertices[i].y - localVertices[j].y;
            float dz = localVertices[i].z - localVertices[j].z;
            float d = std::sqrt(dx*dx + dy*dy + dz*dz);
            if (std::abs(d - edgeLen) < tol) {
                edgeIndices.emplace_back(i, j);
            }
        }
    }
}
