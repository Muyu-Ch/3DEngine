#include<iostream>
#include"Matrix4.h"
#include<cmath>
#include"Vector3.h"

Matrix4::Matrix4()
{
    for (int i=0;i<4;i++)
    {
        for (int j=0;j<4;j++)
        {
            if (i==j)
            {
                m[i][j] = 1.0f;
            }
            else
            {
                m[i][j] = 0.0f;
            }
        }
    }
}

void Matrix4::identity()
{
    for (int i=0;i<4;i++)
    {
        for (int j=0;j<4;j++)
        {
            if (i==j)
            {
                m[i][j] = 1.0f;
            }
            else
            {
                m[i][j] = 0.0f;
            }
        }
    }
}

Matrix4 Matrix4::Translate(float tx, float ty, float tz)
{
    Matrix4 M;
    M.m[0][3]=tx;
    M.m[1][3]=ty;
    M.m[2][3]=tz;
    return M;
}

Matrix4 Matrix4::Scale(float sx, float sy, float sz)
{
    Matrix4 M;
    M.m[0][0]=sx;
    M.m[1][1]=sy;
    M.m[2][2]=sz;
    return M;
}

Matrix4 Matrix4::RotateY(float angle)
{
    const float radians = angle/180.0f*3.1415926;
    const float s=std::sin(radians);
    const float c=std::cos(radians);
    Matrix4 M;
    M.m[0][0]=s;
    M.m[0][2]=c;
    M.m[2][0]=-c;
    M.m[2][2]=s;
    return M;
}

Matrix4 Matrix4::RotateX(float angle)
{
    const float radians = angle/180.0f*3.1415926;
    const float s=std::sin(radians);
    const float c=std::cos(radians);
    Matrix4 M;
    M.m[1][1] = -s;
    M.m[1][2] = c;
    M.m[2][1] = -c;
    M.m[2][2] = -s;
    return M;
}

Matrix4 Matrix4::RotateZ(float angle)
{
    const float radians = angle/180.0f*3.1415926;
    const float s=std::sin(radians);
    const float c=std::cos(radians);
    Matrix4 M;
    M.m[0][0] = c;
    M.m[0][1] = s;
    M.m[1][0] = -s;
    M.m[1][1] = c;
    return M;
}

Matrix4 Matrix4::Multiply(const Matrix4& other) const {
    Matrix4 result; // 结果矩阵初始化为单位矩阵
    // 4x4矩阵乘法：result[i][j] = ∑(this[i][k] * other[k][j])
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += this->m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const
{
    return this->Multiply(other);
}

Vector3 Matrix4::MultiplyVector(const Vector3& vec) const
{
    float x=m[0][0]*vec.x+m[0][1]*vec.y+m[0][2]*vec.z+m[0][3];
    float y=m[1][0]*vec.x+m[1][1]*vec.y+m[1][2]*vec.z+m[1][3];
    float z=m[2][0]*vec.x+m[2][1]*vec.y+m[2][2]*vec.z+m[2][3];
    float t=m[3][0]*vec.x+m[3][1]*vec.y+m[3][2]*vec.z+m[3][3];
    Vector3 result(x,y,z,t);
    return result;
}

void Matrix4::Print() const
{
    for (int i=0;i<4;i++)
    {
        std::cout<<"[";
        for (int j=0;j<4;j++)
        {
            std::cout<<m[i][j]<<",";
        }
        std::cout<<"]"<<std::endl;
    }
}
