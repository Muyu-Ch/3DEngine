#include "Ball.h"
#include <cmath>

Ball::Ball(float radius, int segments, int rings)
{
    // ========== 生成顶点 ==========
    // 布局：顶极点 + rings*segments 个纬圈顶点 + 底极点
    localVertices.clear();

    // 顶极点（索引 0）
    localVertices.emplace_back(0.0f, radius, 0.0f, 1.0f);

    // 纬圈顶点（索引 1 ~ rings*segments）
    for (int i = 1; i <= rings; i++)
    {
        float phi = M_PI * i / (rings + 1);           // 从北极到南极的角度 [0, π]
        float y = radius * std::cos(phi);              // 当前环的 y 坐标
        float r = radius * std::sin(phi);              // 当前环的水平半径

        for (int j = 0; j < segments; j++)
        {
            float theta = 2.0f * M_PI * j / segments;  // 经度角 [0, 2π]
            float x = r * std::cos(theta);
            float z = r * std::sin(theta);
            localVertices.emplace_back(x, y, z, 1.0f);
        }
    }

    // 底极点（索引 rings*segments + 1）
    localVertices.emplace_back(0.0f, -radius, 0.0f, 1.0f);

    // ========== 生成边 ==========
    edgeIndices.clear();

    // 顶极点 → 第一圈的所有顶点
    for (int j = 0; j < segments; j++)
    {
        edgeIndices.emplace_back(0, 1 + j);
    }

    // 纬圈内横向边 + 相邻纬圈间纵向边
    for (int i = 0; i < rings; i++)
    {
        int baseCurr = 1 + i * segments;      // 当前圈起始索引

        for (int j = 0; j < segments; j++)
        {
            int curr = baseCurr + j;
            int nextInRing = baseCurr + (j + 1) % segments;  // 同圈顺时针下一点

            // 横向边（同圈相邻顶点，所有圈都需要）
            edgeIndices.emplace_back(curr, nextInRing);

            // 纵向边：非最后一圈连接到下一圈，最后一圈由后面的底极点循环处理
            if (i < rings - 1)
            {
                int baseNext = 1 + (i + 1) * segments; // 下一圈起始索引
                int below = baseNext + j;
                edgeIndices.emplace_back(curr, below);
            }
        }
    }

    // 最后一圈 → 底极点
    int lastRingStart = 1 + (rings - 1) * segments;
    int bottomPole = static_cast<int>(localVertices.size()) - 1;
    for (int j = 0; j < segments; j++)
    {
        edgeIndices.emplace_back(lastRingStart + j, bottomPole);
    }
}
