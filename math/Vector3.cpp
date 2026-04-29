#include "Vector3.h"
#include<iostream>

Vector3::Vector3(float X, float Y, float Z,float T)
{
    this->x = X;
    this->y = Y;
    this->z = Z;
    this->t = T;
}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z,t+other.t);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z,t-other.t);
}

Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z, -t);
}

Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar,t);
}

float Vector3::length() const
{
    float length = sqrt(x * x + y * y + z * z);
    return length;
}

Vector3 Vector3::normalize() const
{
    float len = length();

    if (len < 0.00001f) {
        return Vector3(0, 0, 0, 0);
    }
    return Vector3(x / len, y / len, z / len, t);
}

float Vector3::dot(const Vector3& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3& other) const
{
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x,
        0  // 叉乘结果是向量，t设为0
    );
}

void Vector3::print() const
{
    std::cout<<"("<<x<<","<<y<<","<<z<<","<<t<<")"<<std::endl;
}
