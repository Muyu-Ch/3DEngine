// render.cpp 补全后
#include "Render.h"
#include<iostream>

Render::Render(int width, int height, float scale)
{
    window=nullptr;
    renderer=nullptr;
    window_width = width;
    window_height = height;
    this->scale = scale;
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

// 以下是空实现（保证编译通过）
void Render::Project(const Vector3& point, int& screen_x, int& screen_y)
{
    // 暂不实现
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
    // 简单的退出逻辑：等待ESC键按下
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
        // 短暂延时，避免CPU占用过高
        SDL_Delay(10);
    }
}
