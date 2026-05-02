// render.cpp
#include "Render.h"
#include "Point.h"
#include<iostream>

Render::Render(int width, int height, float fov)
{
    window=nullptr;
    renderer=nullptr;
    window_width = width;
    window_height = height;
    this->fov = fov;
    this->scale = 1000;
}

Render::~Render()
{
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
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
        SDL_WINDOW_SHOWN            // 显示窗口
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

void Render::Project(const Vector3& point3d, Point& point2d)
{
    double rad = fov * 3.14159 / 180.0;
    double scale = 1.0 / tan(rad / 2);
    if (point3d.z>0)
    {
        point2d.x = window_width / 2 + static_cast<int>((double)point3d.x/point3d.z * scale * this->scale);
        point2d.y = window_height / 2 - static_cast<int>((double)point3d.y/point3d.z * scale * this->scale);
    }
}

void Render::DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (x<0 || x>window_width || y<0 || y>window_height)
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

void Render::DrawLine(Point point1, Point point2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    int x1=point1.x;
    int x2=point2.x;
    int y1=point1.y;
    int y2=point2.y;

    bool isOutLeft = (x1 < 0 && x2 < 0);
    bool isOutRight = (x1 > window_width && x2 > window_width);
    bool isOutTop = (y1 < 0 && y2 < 0);
    bool isOutBottom = (y1 > window_height && y2 > window_height);
    if (isOutLeft || isOutRight || isOutTop || isOutBottom)
    {
        return;
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
