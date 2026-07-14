// render.cpp
#include "Render.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif

// 获取可执行文件所在目录（跨平台）
static std::string GetExecutableDir()
{
    char path[4096] = {0};
#ifdef _WIN32
    GetModuleFileNameA(NULL, path, sizeof(path));
#elif __APPLE__
    uint32_t size = sizeof(path);
    _NSGetExecutablePath(path, &size);
    realpath(path, path);
#else
    // Linux: 通过 /proc/self/exe 获取
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len != -1) path[len] = '\0';
#endif
    std::string full(path);
    return full.substr(0, full.find_last_of("/\\"));
}

Render::Render(int width, int height, float fov)
{
    window=nullptr;
    renderer=nullptr;
    font=nullptr;
    window_width = width;
    window_height = height;
    this->fov = fov;
    this->scale = 1000;
    this->isOrtho = false;
    this->orthoScale = 0.5f;  // 正交投影缩放系数（像素/单位）
}

Render::~Render()
{
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

bool Render::Init()
{
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL初始化失败：" << SDL_GetError() << std::endl;
        return false;
    }

    // 创建窗口
    window = SDL_CreateWindow(
        "3D Engine",                // 窗口标题
        SDL_WINDOWPOS_CENTERED,     // 窗口居中
        SDL_WINDOWPOS_CENTERED,
        window_width,               // 宽度
        window_height,              // 高度
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE  // 显示窗口 + 可拖拽缩放
    );
    if (!window)
    {
        std::cerr << "窗口创建失败：" << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "渲染器创建失败：" << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "窗口创建成功！" << std::endl;
    return true;
}

void Render::SetBackgroundColor(Uint8 red, Uint8 green, Uint8 blue)
{
    bgR=red;
    bgG=green;
    bgB=blue;
}

void Render::SetProjection(bool ortho)
{
    isOrtho = ortho;
}

bool Render::IsOrtho() const
{
    return isOrtho;
}

void Render::Project(const Vector3& point3d, Point& point2d)
{
    if (isOrtho)
    {
        // 正交投影：直接映射，不除以z，不裁剪近平面
        // 超屏由后续2D Cohen-Sutherland 裁剪处理
        point2d.x = window_width / 2 + static_cast<int>(point3d.x * orthoScale);
        point2d.y = window_height / 2 - static_cast<int>(point3d.y * orthoScale);
    }
    else
    {
        // 透视投影：除以z产生近大远小效果
        double rad = fov * 3.14159 / 180.0;
        double scale = 1.0 / tan(rad / 2);
        if (point3d.z >= NEAR_PLANE)
        {
            point2d.x = window_width / 2 + static_cast<int>((double)point3d.x/point3d.z * scale * this->scale);
            point2d.y = window_height / 2 - static_cast<int>((double)point3d.y/point3d.z * scale * this->scale);
        }
        else
        {
            // 顶点在近平面后方，设为无效坐标（后续裁剪应避免走到这里）
            point2d.x = -10000;
            point2d.y = -10000;
        }
    }
    point2d.name = point3d.name; // 保留顶点名称
}

bool Render::ClipSegmentToNearPlane(Vector3& v1, Vector3& v2)
{
    bool v1_inside = v1.z >= NEAR_PLANE;
    bool v2_inside = v2.z >= NEAR_PLANE;

    // 情况1：两端点都在近平面之前 —— 无需裁剪
    if (v1_inside && v2_inside)
    {
        return true;
    }

    // 情况2：两端点都在近平面之后 —— 完全不可见，丢弃
    if (!v1_inside && !v2_inside)
    {
        return false;
    }

    // 情况3：一个在前一个在后 —— 需要裁剪
    // 将 v1 统一为"内侧"（在前面的那个）
    if (!v1_inside)
    {
        std::swap(v1, v2);
        // 现在 v1 在近平面之前，v2 在近平面之后
    }

    // 计算插值参数 t：使得 v1.z + t * (v2.z - v1.z) = NEAR_PLANE
    float t = (NEAR_PLANE - v1.z) / (v2.z - v1.z);

    // 在近平面处裁剪 v2
    v2.x = v1.x + t * (v2.x - v1.x);
    v2.y = v1.y + t * (v2.y - v1.y);
    v2.z = NEAR_PLANE;
    // v2.t (w分量) 也保持线性插值
    v2.t = v1.t + t * (v2.t - v1.t);

    return true;
}

// 辅助函数：用平面 n·P >= 0 裁剪线段（n 指向内侧）
// 返回值：true = 有可见部分，false = 完全在平面外侧
static bool ClipAgainstPlane(Vector3& a, Vector3& b, float nx, float ny, float nz)
{
    float d1 = nx * a.x + ny * a.y + nz * a.z;
    float d2 = nx * b.x + ny * b.y + nz * b.z;

    bool in1 = d1 >= 0.0f;
    bool in2 = d2 >= 0.0f;

    if (in1 && in2) return true;       // 两端都在内侧
    if (!in1 && !in2) return false;    // 两端都在外侧，完全不可见

    // 一端在内一端在外：将 a 统一为内侧点
    if (!in1)
    {
        std::swap(a, b);
        std::swap(d1, d2);
    }

    // 插值裁剪：d1 >= 0, d2 < 0, 找 d=0 的交点
    float t = d1 / (d1 - d2);

    // 如果外侧端点距离平面非常远（t 接近 0），裁剪后只留下极短的线段
    // 若同一点连接的多条边都这样裁剪，会在屏幕边缘形成放射状"长线"伪影
    // 对这种边直接丢弃，避免视觉伪影
    if (t < 0.02f) return false;

    b.x = a.x + t * (b.x - a.x);
    b.y = a.y + t * (b.y - a.y);
    b.z = a.z + t * (b.z - a.z);
    b.t = a.t + t * (b.t - a.t);
    return true;
}

bool Render::ClipSegmentToFrustum(Vector3& v1, Vector3& v2)
{
    // 正交投影：视体是长方体，无边界面扩展
    // 跳过近平面和侧锥面裁剪，超屏由后续2D Cohen-Sutherland 裁剪处理
    if (isOrtho)
        return true;

    // 第1步：近平面裁剪（仅透视投影）
    if (!ClipSegmentToNearPlane(v1, v2))
        return false;

    // 第2步：计算视锥体侧面边界系数
    // 投影公式: screen_x = ww/2 + (x/z) * (1/tan(fov/2)) * scale
    // 由 screen_x ∈ [0, ww] 导出: |x| <= z * (ww/2) * tan(fov/2) / scale
    float tanHalfFov = std::tan(fov * 3.14159f / 180.0f / 2.0f);
    float hFactor = (window_width / 2.0f) * tanHalfFov / this->scale;   // 水平半宽度系数
    float vFactor = (window_height / 2.0f) * tanHalfFov / this->scale;  // 垂直半高度系数

    // 第3步：依次对左、右、底、顶四个视锥面做 3D 裁剪
    //  左平面:  x + hFactor*z >= 0  （法线指向右侧）
    //  右平面: -x + hFactor*z >= 0  （法线指向左侧）
    //  底平面:  y + vFactor*z >= 0  （法线指向上方）
    //  顶平面: -y + vFactor*z >= 0  （法线指向下方）
    if (!ClipAgainstPlane(v1, v2,  1.0f, 0.0f, hFactor)) return false;  // 左
    if (!ClipAgainstPlane(v1, v2, -1.0f, 0.0f, hFactor)) return false;  // 右
    if (!ClipAgainstPlane(v1, v2, 0.0f,  1.0f, vFactor)) return false;  // 底
    if (!ClipAgainstPlane(v1, v2, 0.0f, -1.0f, vFactor)) return false;  // 顶

    return true;
}

void Render::Draw3DLine(Vector3 v1, Vector3 v2,
    Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    // 先对3D线段进行完整视锥体裁剪（近平面 + 4个侧面）
    if (!ClipSegmentToFrustum(v1, v2))
    {
        return;  // 整条线段在视锥体之外，不绘制
    }

    // 投影到2D屏幕坐标
    Point p1, p2;
    Project(v1, p1);
    Project(v2, p2);

    // 绘制2D线段
    DrawLine(p1, p2, r, g, b, a);
}

void Render::Draw3DLines(
    const std::vector<std::pair<Vector3, Vector3>>& lines,
    Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    for (size_t i = 0; i < lines.size(); i++)
    {
        Draw3DLine(lines[i].first, lines[i].second, r, g, b, a);
    }
}

void Render::DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (x<0 || x>=window_width || y<0 || y>=window_height)
    {
        return;
    }
    if (!renderer)
    {
        std::cout<<"渲染器未初始化！"<<std::endl;
        return;
    }
    // 3. 设置绘制颜色（RGBA：r/g/b/透明度，透明度255为不透明）
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    // 4. 画单个像素点
    SDL_RenderDrawPoint(renderer, x, y);
}

// Cohen-Sutherland 线段裁剪：将线段裁剪到屏幕矩形内
// 返回值：true = 有可见部分，false = 完全不可见
static bool CohenSutherlandClip(int& x1, int& y1, int& x2, int& y2,
                                int winW, int winH)
{
    const int INSIDE = 0;
    const int LEFT   = 1;
    const int RIGHT  = 2;
    const int BOTTOM = 4;
    const int TOP    = 8;

    auto outCode = [winW, winH](int x, int y) {
        int code = INSIDE;
        if (x < 0)       code |= LEFT;
        if (x >= winW)   code |= RIGHT;
        if (y < 0)       code |= TOP;
        if (y >= winH)   code |= BOTTOM;
        return code;
    };

    int code1 = outCode(x1, y1);
    int code2 = outCode(x2, y2);

    while (true)
    {
        // 两端都在窗口内
        if ((code1 | code2) == 0) return true;
        // 两端都在窗口外且在同一侧 —— 完全不可见
        if (code1 & code2) return false;

        // 选一个在窗口外的端点
        int codeOut = (code1 != 0) ? code1 : code2;
        int x=0, y=0;

        if (codeOut & TOP)
        {
            x = x1 + (int)((long long)(x2 - x1) * (0 - y1) / (y2 - y1));
            y = 0;
        }
        else if (codeOut & BOTTOM)
        {
            x = x1 + (int)((long long)(x2 - x1) * (winH - 1 - y1) / (y2 - y1));
            y = winH - 1;
        }
        else if (codeOut & RIGHT)
        {
            y = y1 + (int)((long long)(y2 - y1) * (winW - 1 - x1) / (x2 - x1));
            x = winW - 1;
        }
        else if (codeOut & LEFT)
        {
            y = y1 + (int)((long long)(y2 - y1) * (0 - x1) / (x2 - x1));
            x = 0;
        }

        if (codeOut == code1)
        {
            x1 = x; y1 = y;
            code1 = outCode(x1, y1);
        }
        else
        {
            x2 = x; y2 = y;
            code2 = outCode(x2, y2);
        }
    }
}

void Render::DrawLine(Point point1, Point point2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int x1=point1.x;
    int x2=point2.x;
    int y1=point1.y;
    int y2=point2.y;

    // 使用 Cohen-Sutherland 算法将线段裁剪到屏幕范围内
    if (!CohenSutherlandClip(x1, y1, x2, y2, window_width, window_height))
    {
        return;  // 线段完全在屏幕外，不绘制
    }

    if (!renderer)
    {
        std::cout<<"渲染器未初始化！"<<std::endl;
        return;
    }

    int delta_x = x2 - x1;
    int delta_y = y2 - y1;

    int min_x=std::min(x1,x2);
    int max_x=std::max(x1,x2);
    int min_y=std::min(y1,y2);
    int max_y=std::max(y1,y2);

    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    if (abs(delta_x) >= abs(delta_y))
    {
        double d=(double)delta_y/delta_x;

        if (min_x==x1)
        {
            int y;
            for (int x=min_x; x<=max_x; x++)
            {
                y=round((x-min_x)*d+y1);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
        else
        {
            for (int x=min_x; x<=max_x; x++)
            {
                int y=round((x-min_x)*d+y2);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }

    else
    {
        double d=(double)delta_x/delta_y;

        if (min_y==y1)
        {
            int x;
            for (int y=min_y; y<=max_y; y++)
            {
                x=round((y-min_y)*d+x1);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
        else
        {
            for (int y=min_y; y<=max_y; y++)
            {
                int x=round((y-min_y)*d+x2);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

void Render::DrawLines(
        std::vector<std::pair<Point*,Point*> >& lines,
        Uint8 r, Uint8 g, Uint8 b, Uint8 a
        )
{
    for (size_t i=0; i<lines.size(); i++)
    {
        DrawLine(*lines[i].first, *lines[i].second,r,g,b,a);
    }
}

void Render::Clear()
{
    if (!renderer) return;
    // 设置背景色为黑色
    SDL_SetRenderDrawColor(renderer, bgR, bgG, bgB, 255);
    // 清空渲染器缓冲区
    SDL_RenderClear(renderer);
}

void Render::Present()
{
    if (!renderer)
    {
        std::cerr << "渲染器未初始化，无法刷新画面！" << std::endl;
        return;
    }
    // 把渲染器中绘制的内容显示到窗口
    SDL_RenderPresent(renderer);
}

// ========== 文字渲染实现 ==========

bool Render::LoadFont(const char* fontPath, int fontSize)
{
    if (TTF_Init() < 0)
    {
        std::cerr << "TTF初始化失败：" << TTF_GetError() << std::endl;
        return false;
    }

    // 相对路径 → 转为基于可执行文件目录的绝对路径
    std::string fullPath = fontPath;
    if (fontPath[0] != '/' && fontPath[0] != '\0')
    {
        fullPath = GetExecutableDir() + "/" + fontPath;
    }

    font = TTF_OpenFont(fullPath.c_str(), fontSize);
    if (!font)
    {
        std::cerr << "字体加载失败（" << fullPath << "）：" << TTF_GetError() << std::endl;
        return false;
    }
    std::cout << "字体加载成功：" << fullPath << std::endl;
    return true;
}

void Render::DrawText(const char* text, int x, int y,
                      Uint8 r, Uint8 g, Uint8 b)
{
    if (!font || !renderer) return;

    SDL_Color color = {r, g, b, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

void Render::DrawText(const std::string& text, int x, int y,
                      Uint8 r, Uint8 g, Uint8 b)
{
    DrawText(text.c_str(), x, y, r, g, b);
}

void Render::HandleResize(int newWidth, int newHeight)
{
    window_width = newWidth;
    window_height = newHeight;
}

void Render::WaitQuit()
{
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            // 按下ESC键退出
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
            // 点击窗口关闭按钮退出
            else if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_Delay(10);
    }
}

void Render::DrawName(const Point& point, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    std::string nameStr(1, point.name); // 将字符转换为字符串
    DrawText(nameStr.c_str(), point.x, point.y, r, g, b);
}