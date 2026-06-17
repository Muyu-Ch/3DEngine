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

    // 计算右轴，并防止万向节锁死（Front 平行于 Up 时叉乘为零向量）
    Vector3 xAxis = Up.cross(zAxis);
    if (xAxis.length() < 0.0001f)
    {
        // Front 接近 ±Up：换一个世界轴来计算右轴
        Vector3 worldX(1.0f, 0.0f, 0.0f, 0.0f);
        Vector3 worldZ(0.0f, 0.0f, 1.0f, 0.0f);
        Vector3 ref = (std::abs(zAxis.dot(worldX)) < 0.99f) ? worldX : worldZ;
        xAxis = ref.cross(zAxis);
    }
    xAxis = xAxis.normalize();

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
    // 将Front投影到水平面（XZ平面），W/S只沿水平方向移动
    Vector3 frontHoriz = Vector3(Front.x, 0.0f, Front.z);
    float horizLen = frontHoriz.length();
    if (horizLen < 0.0001f)
    {
        // Front接近正上方/正下方时，退化为看向Z轴正方向
        frontHoriz = Vector3(0.0f, 0.0f, 1.0f);
    }
    else
    {
        frontHoriz = frontHoriz * (1.0f / horizLen);  // 归一化
    }

    // 水平右方向：世界Y轴 叉乘 水平前方向
    Vector3 worldUp(0.0f, 1.0f, 0.0f);
    Vector3 camRight = worldUp.cross(frontHoriz).normalize();
    Vector3 camUp(0.0f, 1.0f, 0.0f);  // 上下移动始终沿世界Y轴

    Vector3 movement = camRight   * (speed.x / FPS)
                     + frontHoriz * (speed.z / FPS)
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

Vector3 Camera::getFront()
{
    return Front;
}