// main.cpp（测试入口）
#include <iostream>
#include<vector>
#include "Render.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Point.h"
#include <SDL2/SDL.h>


int main()
{
    // 创建Render对象，默认800x600窗口
    Render render(1000,1000,200.0f);

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

    const Vector3 Center(0.0f, 0.0f, 450.0f);
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

        Matrix4 translateToOrigin = Matrix4::Translate(-Center.x, -Center.y, -Center.z);
        // 步骤2：绕Y轴旋转矩阵（角度逐步增加）
        Matrix4 rotateY = Matrix4::RotateY(rotateAngle);
        Matrix4 rotateX = Matrix4::RotateX(rotateAngle);
        Matrix4 rotateZ = Matrix4::RotateZ(rotateAngle);
        // 步骤3：平移矩阵 - 把立方体移回原位置
        Matrix4 translateBack = Matrix4::Translate(Center.x, Center.y, Center.z);

        Matrix4 transform = translateBack*
                                //rotateX*
                                //rotateZ*
                                rotateY*
                                translateToOrigin;

        for (size_t i=0;i<vectors.size();i++)
        {
            vectors[i]=transform.MultiplyVector(vectors[i]);
        }

        render.Clear();

        std::vector<Point>points;

        for (int i=0;i<8;i++)
        {
            Point* newPoint = new Point();
            render.Project(vectors[i],*newPoint);
            points.push_back(*newPoint);
        }


        std::vector<std::pair<Point*,Point*>>lines={
            std::make_pair(&points[0],&points[1]),
        };

        // for (int i=0;i<4;i++)
        // {
        //     lines.push_back(std::make_pair(&points[i],&points[(i+1)%4]));
        //     lines.push_back(std::make_pair(&points[(i+4)],&points[(i+1)%4+4]));
        //     lines.push_back(std::make_pair(&points[i],&points[i+4]));
        // }
        //立方体绘制

        for (size_t i=0;i<points.size();i++)
        {
            for (size_t j=i+1;j<points.size();j++)
            {
                lines.push_back(std::make_pair(&points[i],&points[j]));
            }
        }//立方体所有顶点相邻绘制

        render.DrawLines(lines);

        // 5. 刷新显示（把新画面展示到窗口）
        render.Present();

        // 控制帧率（约60帧/秒，避免画面太快）
        SDL_Delay(16);
    }

    std::cout<<"程序退出"<<std::endl;

    return 0;
}
