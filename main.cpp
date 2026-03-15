// main.cpp（测试入口）
#include "Render.h"
#include <iostream>

int main()
{
    // 创建Render对象，默认800x600窗口
    Render render(1000,1000,100.0f);

    // 初始化（创建窗口）
    if (!render.Init())
    {
        std::cerr << "Render初始化失败！" << std::endl;
        return -1;
    }

    bool isRunning = true;    // 主循环开关
    SDL_Event event;          // 事件对象（检测按键/关闭窗口）
    Vector3 point1(300,300,10);
    Vector3 point2(300,-300,10);
    Vector3 point3(-300,300,10);
    Vector3 point4(-300,-300,10);
    Vector3 point5(300,300,310);
    Vector3 point6(300,-300,310);
    Vector3 point7(-300,300,310);
    Vector3 point8(-300,-300,310);
    float Dz=1;

    while (isRunning)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT) // 点击窗口关闭按钮
                isRunning = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) // 按ESC
                isRunning = false;
        }

        point1.z+=Dz;
        point2.z+=Dz;
        point3.z+=Dz;
        point4.z+=Dz;
        point5.z+=Dz;
        point6.z+=Dz;
        point7.z+=Dz;
        point8.z+=Dz;

        render.Clear();

        int screenX1, screenY1;
        int screenX2, screenY2;
        int screenX3, screenY3;
        int screenX4, screenY4;
        int screenX5, screenY5;
        int screenX6, screenY6;
        int screenX7, screenY7;
        int screenX8, screenY8;
        render.Project(point1, screenX1, screenY1);
        render.Project(point2, screenX2, screenY2);
        render.Project(point3, screenX3, screenY3);
        render.Project(point4, screenX4, screenY4);
        render.Project(point5, screenX5, screenY5);
        render.Project(point6, screenX6, screenY6);
        render.Project(point7, screenX7, screenY7);
        render.Project(point8, screenX8, screenY8);

        // 画一个5x5的白色方块（方便看）
        for (int dx = -4; dx <= 4; dx++)
        {
            for (int dy = -4; dy <= 4; dy++)
            {
                render.DrawPixel(screenX1 + dx, screenY1 + dy, 255, 255, 255);
                render.DrawPixel(screenX2 + dx, screenY2 + dy, 255, 255, 255);
                render.DrawPixel(screenX3 + dx, screenY3 + dy, 255, 255, 255);
                render.DrawPixel(screenX4 + dx, screenY4 + dy, 255, 255, 255);
                render.DrawPixel(screenX5 + dx, screenY5 + dy, 255, 255, 255);
                render.DrawPixel(screenX6 + dx, screenY6 + dy, 255, 255, 255);
                render.DrawPixel(screenX7 + dx, screenY7 + dy, 255, 255, 255);
                render.DrawPixel(screenX8 + dx, screenY8 + dy, 255, 255, 255);
            }
        }

        // 5. 刷新显示（把新画面展示到窗口）
        render.Present();

        // 控制帧率（约60帧/秒，避免画面太快）
        SDL_Delay(16);
    }

    std::cout<<"程序退出"<<std::endl;

    return 0;
}