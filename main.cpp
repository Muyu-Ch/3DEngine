#include <iostream>
#include"math/Vector3.h"
#include"math/Matrix4.h"
int main()
{
    Vector3 v1(1,2,3);
    Vector3 v2(4,5,6);
    std::cout<<"v1:";
    v1.print();
    std::cout<<"v2:";
    v2.print();
    Vector3 v3=v1+v2;
    std::cout<<"v3=v1+v2:";
    v3.print();
    std::cout<<std::endl;;

    Matrix4 m=Matrix4::Translate(1,1,1);//平移测试
    std::cout<<"平移矩阵:"<<std::endl;
    m.Print();
    v1=m.MultiplyVector(v1);
    std::cout<<"平移后的v1:";
    v1.print();
    m=Matrix4::Scale(2,2,2);
    std::cout<<"拉伸矩阵"<<std::endl;
    m.Print();
    v2=m.MultiplyVector(v2);
    std::cout<<"拉伸后的v2:";
    v2.print();
    m=Matrix4::RotateY(-90);
    std::cout<<"旋转矩阵:"<<std::endl;
    m.Print();
    v3=m.MultiplyVector(v3);
    std::cout<<"旋转后的v3:";
    v3.print();
    return 0;
}
