#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Matrix4.h"

class Camera {
public:
    Vector3 Position;  // 相机位置
    Vector3 Front;     // 相机朝向
    Vector3 Up;        // 相机上方向
    Matrix4 ViewM;     // 视图矩阵（设为public方便访问）

    // 构造函数
    Camera();
    Camera(const Vector3& position, const Vector3& front, const Vector3& up);

    // 设置相机位置（自动更新视图矩阵）
    void setPosition(const Vector3& position);
    // 设置相机朝向（自动更新视图矩阵）
    void setFront(const Vector3& front);
    // 计算视图矩阵（核心：适配你的行优先矩阵+右手系）
    void updateViewM();

    Vector3 getPosition();

private:
    // 辅助函数：确保计算精度
    float clamp(float val, float min, float max) {
        return val < min ? min : (val > max ? max : val);
    }
};

#endif // CAMERA_H
