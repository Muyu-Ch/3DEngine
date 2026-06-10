// main.cpp（测试入口）
#include <iostream>
#include<vector>
#include "Render.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Point.h"
#include "Camera.h"
#include <SDL2/SDL.h>


int main()
{
    //初始化部分
    // 创建Render对象，默认800x600窗口
    Render render(1000,1000,100.0f);
    // 初始化（创建窗口）
    if (!render.Init())
    {
        std::cerr << "Render初始化失败！" << std::endl;
        return -1;
    }
    render.SetBackgroundColor(WHITE);
    bool isRunning = true;    // 主循环开关
    SDL_Event event; // 事件对象
    float FPS=60.0;

    //对象数据初始化
    //立方体八个顶点坐标
    std::vector<Vector3> vertices={
        Vector3(150,150,500),
        Vector3(150,-150,500),
        Vector3 (-150,-150,500),
        Vector3(-150,150,500),
        Vector3(150,150,800),
        Vector3(150,-150,800),
        Vector3(-150,-150,800),
        Vector3(-150,150,800)
    };
    //立方体中心
    Vector3 Center=Vector3();
    Center.center(vertices);
    float rotateAngle=45;
    float X=0;

    //变化量
    //速度的单位是单位/s
    float dAngle=60.0f;
    float dx=100.0;
    float cameraUpangle=0.0;
    float cameraWaveangle=0.0;
    Vector3 cameraSpeed=Vector3(0.0,0.0,0.0,0);
    Matrix4 cameraWave = Matrix4::RotateY(cameraWaveangle);

    //摄像头位置+方向
    Vector3 cameraPosition=Vector3(0,0,0,1);
    Vector3 cameraFount = Vector3(0.0f, 0.0f, 1.0f, 0.0f);

    //摄像头实体
    Camera camera;
    camera.setPosition(cameraPosition);
    camera.setFront(cameraFount);

    while (isRunning)
    {
        const Uint8* keys=SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_A])//X方向
        {cameraSpeed.x=-50.0;}
        else if (keys[SDL_SCANCODE_D])
        {cameraSpeed.x=50.0;}
        else
        {cameraSpeed.x=0.0;}

        if (keys[SDL_SCANCODE_W])//Z方向
        {cameraSpeed.z=50.0;}
        else if (keys[SDL_SCANCODE_S])
        {cameraSpeed.z=-50.0;}
        else
        {cameraSpeed.z=0;}

        if (keys[SDL_SCANCODE_Q])//Y方向
        {cameraSpeed.y=50.0;}
        else if (keys[SDL_SCANCODE_E])
        {cameraSpeed.y=-50.0;}
        else
        {cameraSpeed.y=0;}

        if (keys[SDL_SCANCODE_I])//抬头
        {cameraUpangle=30.0;}
        else if (keys[SDL_SCANCODE_K])
        {cameraUpangle=-30.0;}
        else
        {cameraWaveangle=0;}

        if (keys[SDL_SCANCODE_J])//Y方向
        {cameraWaveangle=30.0;}
        else if (keys[SDL_SCANCODE_L])
        {cameraWaveangle=-30.0;}
        else
        {cameraWaveangle=0;}
        std::vector<Vector3> vectors=vertices;


        //物体变化量
        rotateAngle+=dAngle/FPS;
        X+=dx/FPS;

        //相机变化量
        camera.setPosition(camera.getPosition()+cameraSpeed);//相机按照自定义方向移动

        //cameraFount=cameraWave.MultiplyVector(cameraFount);
        //camera.setFront(cameraFount);

        //旋转矩阵
        Matrix4 rotateY = Matrix4::RotateY(rotateAngle);
        Matrix4 rotateX = Matrix4::RotateX(rotateAngle);
        Matrix4 rotateZ = Matrix4::RotateZ(rotateAngle);

        //平移矩阵
        Matrix4 translateX=Matrix4::Translate(X,0,0);

        //复原矩阵
        Matrix4 translateToOrigin = Matrix4::Translate(-Center.x, -Center.y, -Center.z);
        Matrix4 translateBack = Matrix4::Translate(Center.x, Center.y, Center.z);

        Matrix4 transform ={
            camera.ViewM*
            //translateX*
                translateBack*
                    //rotateX*
                        //rotateZ*
                            rotateY*
                                translateToOrigin
        };


        for (size_t i=0;i<vectors.size();i++)
        {
            vectors[i]=transform.MultiplyVector(vectors[i]);
        }

        // 使用3D线段构建边（所有顶点两两连线 = 立方体所有边+对角线）
        std::vector<std::pair<Vector3, Vector3>> edges3D;
        for (size_t i = 0; i < vectors.size(); i++)
        {
            for (size_t j = i + 1; j < vectors.size(); j++)
            {
                edges3D.emplace_back(vectors[i], vectors[j]);
            }
        }

        // 绘制部分（Draw3DLines 内部完成近平面裁剪 → 投影 → 2D绘制）
        render.Clear();
        render.Draw3DLines(edges3D, BLACK);
        render.Present();

        //退出引擎
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT) // 点击窗口关闭按钮
                isRunning = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) // 按ESC
                isRunning = false;
        }

        // 控制帧率
        SDL_Delay(1000/FPS);
    }

    std::cout<<"程序退出"<<std::endl;

    return 0;
}
