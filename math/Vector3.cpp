#include "Vector3.h"
#include<iostream>

Vector3::Vector3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector3::Vector3(float X, float Y, float Z,float T)
{
    this->x = X;
    this->y = Y;
    this->z = Z;
    this->t = T;
}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

void Vector3::print() const
{
    std::cout<<"("<<x<<","<<y<<","<<z<<")"<<std::endl;
}
