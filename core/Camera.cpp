#include "Camera.h"
#include <cmath>

// 默认构造：相机在原点，朝向Z轴正方向，上方向Y轴
Camera::Camera() {
    Position = Vector3(0.0f, 0.0f, 0.0f, 1.0f);
    Front = Vector3(0.0f, 0.0f, 1.0f, 0.0f);   // 朝向Z+
    Up = Vector3(0.0f, 1.0f, 0.0f, 0.0f);      // 上方向Y+
    ViewM = Matrix4();
    updateViewM();
}

// 自定义构造
Camera::Camera(const Vector3& position, const Vector3& front, const Vector3& up) {
    Position = position;
    Front = front.normalize();
    Up = up.normalize();
    ViewM = Matrix4();
    updateViewM();
}

// 设置相机位置
void Camera::setPosition(const Vector3& position) {
    Position = position;
    updateViewM();
}

// 设置相机朝向
void Camera::setFront(const Vector3& front) {
    Front = front.normalize();
    updateViewM();
}

void Camera::updateViewM() {
    // 1. 计算相机三个方向轴
    Vector3 zAxis = Front.normalize();
    Vector3 xAxis = Up.cross(zAxis).normalize();
    Vector3 yAxis = zAxis.cross(xAxis).normalize();

    // 2. 先重置为单位矩阵
    ViewM.identity();

    // 3. 手动赋值（完全适配你的矩阵格式）
    ViewM.m[0][0] = xAxis.x;
    ViewM.m[0][1] = xAxis.y;
    ViewM.m[0][2] = xAxis.z;
    ViewM.m[0][3] = -xAxis.dot(Position);

    ViewM.m[1][0] = yAxis.x;
    ViewM.m[1][1] = yAxis.y;
    ViewM.m[1][2] = yAxis.z;
    ViewM.m[1][3] = -yAxis.dot(Position);

    ViewM.m[2][0] = zAxis.x;
    ViewM.m[2][1] = zAxis.y;
    ViewM.m[2][2] = zAxis.z;
    ViewM.m[2][3] = -zAxis.dot(Position);

    ViewM.m[3][0] = 0.0f;
    ViewM.m[3][1] = 0.0f;
    ViewM.m[3][2] = 0.0f;
    ViewM.m[3][3] = 1.0f;
}

void Camera::move(int FPS)
{
    // 相机本地方向移动：将移动量投影到镜头的右/前/上方向
    Vector3 camRight   = Up.cross(Front).normalize();   // 镜头右方向
    Vector3 camForward = Front.normalize();                    // 镜头前方向
    Vector3 camUp      = Up.normalize();                       // 镜头上方向

    Vector3 movement = camRight   * (speed.x / FPS)
                     + camForward * (speed.z / FPS)
                     + camUp      * (speed.y / FPS);

    setPosition(getPosition() + movement);
}

void Camera::turn(float FPS)
{
    // 水平旋转（Yaw）：绕世界 Y 轴旋转镜头朝向
    float yawAngle = angleSpeed.y / FPS;
    Matrix4 yawMatrix = Matrix4::RotateY(yawAngle);
    Front = yawMatrix.MultiplyVector(Front);

    // 垂直旋转（Pitch）：绕镜头的右轴旋转镜头朝向
    float pitchAngle = angleSpeed.x / FPS;
    Vector3 rightAxis = Up.cross(Front).normalize();
    Matrix4 pitchMatrix = Matrix4::RotateAxis(rightAxis, pitchAngle);
    Front = pitchMatrix.MultiplyVector(Front);

    setFront(Front);
}

Vector3 Camera::getPosition()
{
    return Position;
}