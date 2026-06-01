#ifndef INC_3DENGINE_RENDER_H
#define INC_3DENGINE_RENDER_H

#include "Vector3.h"
#include "Point.h"
#include<vector>
// SDL2头文件
#include <SDL2/SDL.h>

#define RGB(r,g,b) r,g,b
#define RGBA(r,g,b,a) r,g,b,a
// 基础色系
#define WHITE      255,255,255
#define BLACK      0,0,0
#define RED        255,0,0
#define GREEN      0,255,0
#define BLUE       0,0,255

// 扩展纯色
#define YELLOW     255,255,0
#define CYAN       0,255,255
#define MAGENTA    255,0,255
#define GRAY       128,128,128
#define LIGHT_GRAY 192,192,192
#define DARK_GRAY  64,64,64

// 常用亮色/暗色
#define ORANGE     255,165,0
#define PURPLE     128,0,128
#define PINK       255,192,203
#define BROWN      139,69,19
#define GOLD       255,215,0
#define SILVER     192,192,192
#define NAVY       0,0,128
#define TEAL       0,128,128
#define OLIVE      128,128,0


class Render
{
private:
    SDL_Window* window;      // SDL窗口指针
    SDL_Renderer* renderer;  // SDL渲染器指针
    int window_width;        // 窗口宽度
    int window_height;       // 窗口高度
    float fov;  // 视角
    float scale; //像素比例尺
    Uint8 bgR=0;
    Uint8 bgG=0;
    Uint8 bgB=0;

public:
    // 构造函数
    Render(int width = 800, int height = 600, float fov = 45.0f);
    // 析构函数（释放资源）
    ~Render();

    // 初始化窗口和渲染器
    bool Init();

    //背景颜色设置
    void SetBackgroundColor(Uint8 red=0, Uint8 green=0, Uint8 blue=0);

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
