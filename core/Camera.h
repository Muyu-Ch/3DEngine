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
    Vector3 speed;
    Vector3 angleSpeed;  // 角度变化速度 (x: pitch上下, y: yaw左右, 单位: 度/秒)
    Vector3 cachedRight; // 缓存上一帧的右轴方向，万向节锁时保持方向连续，避免画面跳转

    // 构造函数
    Camera();
    Camera(const Vector3& position, const Vector3& front, const Vector3& up);

    // 设置相机位置（自动更新视图矩阵）
    void setPosition(const Vector3& position);
    // 设置相机朝向（自动更新视图矩阵）
    void setFront(const Vector3& front);
    // 计算视图矩阵（核心：适配你的行优先矩阵+右手系）
    void updateViewM();

    void move(int FPS);
    void turn(float FPS, bool isOrtho = false);   // 旋转镜头朝向，正交模式下允许完全俯视

    Vector3 getPosition();
    Vector3 getFront();

};

#endif // CAMERA_H
