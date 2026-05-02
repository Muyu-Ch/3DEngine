#ifndef INC_3DENGINE_RENDER_H
#define INC_3DENGINE_RENDER_H

#include "Vector3.h"
#include "Point.h"
#include<vector>
// SDL2头文件
#include <SDL2/SDL.h>

class Render
{
private:
    SDL_Window* window;      // SDL窗口指针
    SDL_Renderer* renderer;  // SDL渲染器指针
    int window_width;        // 窗口宽度
    int window_height;       // 窗口高度
    float fov;  // 视角
    float scale; //像素比例尺

public:
    // 构造函数
    Render(int width = 800, int height = 600, float fov = 45.0f);
    // 析构函数（释放资源）
    ~Render();

    // 初始化窗口和渲染器
    bool Init();

    // 3D坐标→2D屏幕坐标（正交投影）
    void Project(const Vector3& point3d, Point& points2d);

    // 画像素点（指定颜色）
    void DrawPixel(int x, int y,
        Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a=255);


    void DrawLine(Point point1,Point point2,
        Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a=255);

    void DrawLines(
        std::vector<std::pair<Point*,Point*> >& lines,
        Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a=255
        );

    // 清空窗口（黑色背景）
    void Clear();

    // 刷新画面（把绘制的内容显示到窗口）
    void Present();

    // 等待退出（按ESC关闭窗口）
    void WaitQuit();
};

#endif
