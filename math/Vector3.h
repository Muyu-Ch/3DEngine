
#ifndef INC_3DENGINE_VECTOR3_H
#define INC_3DENGINE_VECTOR3_H

class Vector3
{
public:
    float x,y,z,t;

    Vector3();
    Vector3(float X=0,float Y=0,float Z=0,float T=1);

    Vector3 operator+(const Vector3& other)const;
    Vector3 operator-(const Vector3& other)const;

    void print()const;
};


#endif //INC_3DENGINE_VECTOR3_H