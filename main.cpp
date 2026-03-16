// main.cpp（测试入口）
#include "Render.h"
#include <iostream>
#include<vector>

#include "Matrix4.h"

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
    SDL_Event event; // 事件对象（检测按键/关闭窗口）

    std::vector<Vector3> vectors={
        Vector3(150,150,300),
        Vector3(150,-150,300),
        Vector3 (-150,-150,300),
        Vector3(-150,150,300),
        Vector3(150,150,600),
        Vector3(150,-150,600),
        Vector3(-150,-150,600),
        Vector3(-150,150,600)
    };

    const Vector3 cubeCenter(0.0f, 0.0f, 450.0f);
    float rotateAngle=1;

    while (isRunning)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT) // 点击窗口关闭按钮
                isRunning = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) // 按ESC
                isRunning = false;
        }

        Matrix4 translateToOrigin = Matrix4::Translate(-cubeCenter.x, -cubeCenter.y, -cubeCenter.z);
        // 步骤2：绕Y轴旋转矩阵（角度逐步增加）
        Matrix4 rotateY = Matrix4::RotateY(rotateAngle);
        Matrix4 rotateX = Matrix4::RotateX(rotateAngle);
        Matrix4 rotateZ = Matrix4::RotateZ(rotateAngle);
        // 步骤3：平移矩阵 - 把立方体移回原位置
        Matrix4 translateBack = Matrix4::Translate(cubeCenter.x, cubeCenter.y, cubeCenter.z);

        Matrix4 transform = translateBack*
                                rotateX*
                                rotateZ*
                                rotateY*
                                translateToOrigin;

        for (size_t i=0;i<vectors.size();i++)
        {
            vectors[i]=transform.MultiplyVector(vectors[i]);
        }

        render.Clear();

        std::vector<std::pair<int,int>>points;

        for (int i=0;i<8;i++)
        {
            int x,y;
            render.Project(vectors[i],x,y);
            points.push_back(std::make_pair(x,y));
        }

        // 画一个5x5的白色方块（方便看）
        // for (int i=0;i<8;i++)
        // {
        //     int x=points[i].first;
        //     int y=points[i].second;
        //     for (int dx = -4; dx <= 4; dx++)
        //     {
        //         for (int dy = -4; dy <= 4; dy++)
        //         {
        //             render.DrawPixel(x+dx,y+dy);
        //         }
        //     }
        // }

        for (int i=0;i<4;i++)
        {
            render.DrawLine(points[i].first,points[i].second,points[(i+1)%4].first,points[(i+1)%4].second);
            render.DrawLine(points[i+4].first,points[i+4].second,points[(i+1)%4+4].first,points[(i+1)%4+4].second);
            render.DrawLine(points[i].first,points[i].second,points[i+4].first,points[i+4].second);
        }

        // 5. 刷新显示（把新画面展示到窗口）
        render.Present();

        // 控制帧率（约60帧/秒，避免画面太快）
        SDL_Delay(16);
    }

    std::cout<<"程序退出"<<std::endl;

    return 0;
}
