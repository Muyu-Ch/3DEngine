#include "Camera.h"
#include <cmath>

// 默认构造：相机在原点，朝向Z轴正方向，上方向Y轴
Camera::Camera() {
    Position = Vector3(0.0f, 0.0f, 0.0f, 1.0f);
    Front = Vector3(0.0f, 0.0f, 1.0f, 0.0f);   // 朝向Z+
    Up = Vector3(0.0f, 1.0f, 0.0f, 0.0f);      // 上方向Y+
    cachedRight = Vector3(1.0f, 0.0f, 0.0f, 0.0f); // 初始右轴+X
    ViewM = Matrix4();
    updateViewM();
}

// 自定义构造
Camera::Camera(const Vector3& position, const Vector3& front, const Vector3& up) {
    Position = position;
    Front = front.normalize();
    Up = up.normalize();
    cachedRight = Up.cross(Front).normalize();
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

    // 用世界Up叉乘zAxis，当结果够大时更新缓存右轴（可靠方向）
    Vector3 crossXAxis = Up.cross(zAxis);
    float crossLen = crossXAxis.length();
    if (crossLen > 0.01f)
    {
        cachedRight = crossXAxis * (1.0f / crossLen);  // 叉乘可靠，更新缓存
    }
    // 始终用缓存右轴投影到与zAxis垂直的平面，保证方向连续不跳变
    Vector3 xAxis = cachedRight - zAxis * cachedRight.dot(zAxis);
    float len = xAxis.length();
    if (len < 0.0001f)
    {
        // 缓存与zAxis平行（极端边缘情况），回退到世界轴
        Vector3 worldX(1.0f, 0.0f, 0.0f, 0.0f);
        xAxis = worldX - zAxis * worldX.dot(zAxis);
        if (xAxis.length() < 0.0001f)
        {
            Vector3 worldZ(0.0f, 0.0f, 1.0f, 0.0f);
            xAxis = worldZ - zAxis * worldZ.dot(zAxis);
        }
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

void Camera::turn(float FPS, bool isOrtho)
{
    // 水平旋转（Yaw）：始终绕世界 Y 轴（固定竖直轴），保证转头永远是水平的
    float yawAngle = angleSpeed.y / FPS;
    Matrix4 yawMatrix = Matrix4::RotateY(yawAngle);
    Front = yawMatrix.MultiplyVector(Front);

    // 垂直旋转（Pitch）：绕水平右轴（世界Y × Front），保证俯仰轴始终水平
    float pitchAngle = angleSpeed.x / FPS;
    Vector3 worldUp(0.0f, 1.0f, 0.0f);

    // 用世界Y叉乘Front算右轴，够大时更新缓存
    Vector3 crossRight = worldUp.cross(Front);
    float crossLen = crossRight.length();
    if (crossLen > 0.01f)
    {
        cachedRight = crossRight * (1.0f / crossLen);  // 叉乘可靠，更新缓存
    }
    // 始终用缓存右轴投影到水平面，保证方向连续
    Vector3 rightAxis = cachedRight - worldUp * cachedRight.dot(worldUp);
    float len = rightAxis.length();
    if (len < 0.0001f)
    {
        rightAxis = Vector3(1.0f, 0.0f, 0.0f);  // 缓存垂直（极少情况），回退+X
    }
    rightAxis = rightAxis.normalize();

    Matrix4 pitchMatrix = Matrix4::RotateAxis(rightAxis, pitchAngle);
    Vector3 newFront = pitchMatrix.MultiplyVector(Front);

    // 透视模式下限制 pitch 范围（约 ±87°），防止无限绕圈
    // 正交模式下允许完全俯视/仰视（Front.y 可达 ±1.0）
    if (isOrtho || std::abs(newFront.y) < 0.999f)
    {
        Front = newFront;
    }
    // 超过限幅则忽略本次 pitch 旋转

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