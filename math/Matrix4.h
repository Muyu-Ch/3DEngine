#ifndef INC_3DENGINE_MATRIX4_H
#define INC_3DENGINE_MATRIX4_H


// 提前声明 Vector3 类（因为 Matrix4 要用到 Vector3，避免循环包含）
class Vector3;

// 4x4矩阵类：封装3D空间变换的核心逻辑
class Matrix4
{
public:
    // 核心数据：存储4x4矩阵的16个元素（按行优先存储）
    // m[行][列]，比如 m[0][3] 是第一行第四列（平移tx），m[1][1]是第二行第二列（缩放sy）
    float m[4][4];

    // ============== 第一部分：构造函数（初始化矩阵） ==============
    // 1. 默认构造函数：初始化单位矩阵（无参数，最基础的矩阵）
    Matrix4();


    void identity();
    // ============== 第二部分：生成基础变换矩阵（静态方法，直接返回变换矩阵） ==============
    // 1. 生成平移矩阵
    // 参数：tx(x轴平移量), ty(y轴平移量), tz(z轴平移量)
    // 返回：对应的4x4平移矩阵
    static Matrix4 Translate(float tx, float ty, float tz);

    // 2. 生成缩放矩阵
    // 参数：sx(x轴缩放倍数), sy(y轴缩放倍数), sz(z轴缩放倍数)
    // 返回：对应的4x4缩放矩阵
    static Matrix4 Scale(float sx, float sy, float sz);

    // 3. 生成绕Y轴旋转的矩阵（先实现绕Y轴，后续可扩展X/Z轴）
    // 参数：angle 旋转角度（单位：度，方便新手使用，内部转弧度）
    // 返回：对应的4x4旋转矩阵
    static Matrix4 RotateY(float angle);
    // 绕X轴旋转矩阵
    static Matrix4 RotateX(float angle);
    // 绕Z轴旋转矩阵
    static Matrix4 RotateZ(float angle);

    Matrix4 Multiply(const Matrix4& other) const;

    Matrix4 operator*(const Matrix4& other) const;

    Vector3 MultiplyVector(const Vector3& vec) const;

    // ============== 第四部分：辅助功能（可选但实用） ==============
    // 1. 打印矩阵（调试用，能直观看到矩阵的16个值）
    void Print() const;
};

#endif //INC_3DENGINE_MATRIX4_H