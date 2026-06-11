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
    render.SetBackgroundColor(BLACK);
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

        // 相机本地移动方向（相对于镜头朝向）
        float moveForward = 0;   // W/S：前/后
        float moveRight   = 0;   // A/D：左/右
        float moveUp      = 0;   // Q/E：上/下

        if (keys[SDL_SCANCODE_W])
            moveForward = 150.0;
        else if (keys[SDL_SCANCODE_S])
            moveForward = -150.0;

        if (keys[SDL_SCANCODE_D])
            moveRight = 150.0;
        else if (keys[SDL_SCANCODE_A])
            moveRight = -150.0;

        if (keys[SDL_SCANCODE_Q])
            moveUp = 150.0;
        else if (keys[SDL_SCANCODE_E])
            moveUp = -150.0;

        if (keys[SDL_SCANCODE_K])//低头
        {cameraUpangle=30.0;}
        else if (keys[SDL_SCANCODE_I])//抬头
        {cameraUpangle=-30.0;}
        else
        {cameraUpangle=0;}

        if (keys[SDL_SCANCODE_L])//Y方向
        {cameraWaveangle=30.0;}
        else if (keys[SDL_SCANCODE_J])
        {cameraWaveangle=-30.0;}
        else
        {cameraWaveangle=0;}
        std::vector<Vector3> vectors=vertices;


        //物体变化量
        rotateAngle+=dAngle/FPS;
        X+=dx/FPS;

        // 水平旋转（Yaw）：绕世界 Y 轴旋转镜头朝向
        cameraWaveangle *= (1/FPS);
        cameraWave = Matrix4::RotateY(cameraWaveangle);
        cameraFount = cameraWave.MultiplyVector(cameraFount);

        // 垂直旋转（Pitch）：绕镜头的右轴旋转镜头朝向
        cameraUpangle *= (1/FPS);
        Vector3 rightAxis = camera.Up.cross(cameraFount).normalize();
        Matrix4 pitchMatrix = Matrix4::RotateAxis(rightAxis, cameraUpangle);
        cameraFount = pitchMatrix.MultiplyVector(cameraFount);

        camera.setFront(cameraFount);

        // 相机本地方向移动：将移动量投影到镜头的右/前/上方向
        Vector3 camRight   = camera.Up.cross(camera.Front).normalize();   // 镜头右方向
        Vector3 camForward = camera.Front.normalize();                    // 镜头前方向
        Vector3 camUp      = camera.Up.normalize();                       // 镜头上方向

        Vector3 movement = camRight   * (moveRight / FPS)
                         + camForward * (moveForward / FPS)
                         + camUp      * (moveUp / FPS);

        camera.setPosition(camera.getPosition() + movement);

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
                            //rotateY*
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

        // ========== 绘制部分 ==========
        render.Clear();

        // 1. 地面网格（XZ 平面，Y=0，黄色，每 50 单位一条，中心在原点）
        const float GRID_SIZE = 2500.0f;
        const float GRID_STEP = 50.0f;
        std::vector<std::pair<Vector3, Vector3>> gridEdges;

        // 平行于 X 轴的线（在不同 Z 位置）
        for (float z = -GRID_SIZE; z <= GRID_SIZE; z += GRID_STEP)
        {
            Vector3 v1(-GRID_SIZE, 0.0f, z, 1.0f);
            Vector3 v2( GRID_SIZE, 0.0f, z, 1.0f);
            v1 = camera.ViewM.MultiplyVector(v1);
            v2 = camera.ViewM.MultiplyVector(v2);
            gridEdges.emplace_back(v1, v2);
        }

        // 平行于 Z 轴的线（在不同 X 位置）
        for (float x = -GRID_SIZE; x <= GRID_SIZE; x += GRID_STEP)
        {
            Vector3 v1(x, 0.0f, -GRID_SIZE, 1.0f);
            Vector3 v2(x, 0.0f,  GRID_SIZE, 1.0f);
            v1 = camera.ViewM.MultiplyVector(v1);
            v2 = camera.ViewM.MultiplyVector(v2);
            gridEdges.emplace_back(v1, v2);
        }
        render.Draw3DLines(gridEdges, YELLOW);

        // 2. 坐标轴：X=红，Y=绿，Z=蓝
        {
            Vector3 v1(0.0f, 0.0f, 0.0f, 1.0f);
            Vector3 v2(GRID_SIZE, 0.0f, 0.0f, 1.0f);
            v1 = camera.ViewM.MultiplyVector(v1);
            v2 = camera.ViewM.MultiplyVector(v2);
            render.Draw3DLine(v1, v2, RED);
        }
        {
            Vector3 v1(0.0f, 0.0f, 0.0f, 1.0f);
            Vector3 v2(0.0f, GRID_SIZE, 0.0f, 1.0f);
            v1 = camera.ViewM.MultiplyVector(v1);
            v2 = camera.ViewM.MultiplyVector(v2);
            render.Draw3DLine(v1, v2, GREEN);
        }
        {
            Vector3 v1(0.0f, 0.0f, 0.0f, 1.0f);
            Vector3 v2(0.0f, 0.0f, GRID_SIZE, 1.0f);
            v1 = camera.ViewM.MultiplyVector(v1);
            v2 = camera.ViewM.MultiplyVector(v2);
            render.Draw3DLine(v1, v2, BLUE);
        }

        // 3. 立方体
        render.Draw3DLines(edges3D,WHITE);

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
