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
    M.m[0][0]=c;
    M.m[0][2]=s;
    M.m[2][0]=-s;
    M.m[2][2]=c;
    return M;
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
