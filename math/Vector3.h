
#ifndef INC_3DENGINE_VECTOR3_H
#define INC_3DENGINE_VECTOR3_H

#include<cmath>

class Vector3
{
public:
    float x,y,z,t;

    Vector3(float X=0,float Y=0,float Z=0,float T=1);

    Vector3 operator+(const Vector3& other)const;
    Vector3 operator-(const Vector3& other)const;

    Vector3 operator-() const;

    // 2. 标量乘法（向量 × 浮点数，比如 向量 * 0.5）
    Vector3 operator*(float scalar) const;

    // 3. 计算向量长度（模长）
    float length() const;

    // 4. 归一化（返回单位向量，长度为1）
    Vector3 normalize() const;

    // 5. 点乘（内积）→ 视图矩阵平移部分需要
    float dot(const Vector3& other) const;

    // 6. 叉乘（外积）→ 计算相机右/上方向需要
    Vector3 cross(const Vector3& other) const;

    void print()const;
};


#endif //INC_3DENGINE_VECTOR3_H