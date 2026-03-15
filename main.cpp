// main.cpp（测试入口）
#include "Render.h"
#include <iostream>

int main()
{
    // 创建Render对象，默认800x600窗口
    Render render(1000,1000);

    // 初始化（创建窗口）
    if (!render.Init())
    {
        std::cerr << "Render初始化失败！" << std::endl;
        return -1;
    }

    std::cout << "窗口已显示，按ESC键关闭..." << std::endl;
    for (int i=0;i<1000;i++)
    {
        for (int j=0;j<1000;j++)
        {
            render.DrawPixel(i,j,255,255,255);
        }

    }

    render.Present();

    render.WaitQuit();

    return 0;
}
