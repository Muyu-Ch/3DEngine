// main.cpp（测试入口）
#include <iostream>
#include<vector>
#include<string>
#include <SDL2/SDL.h>
#include <format>
#include "Render.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Camera.h"
#include "Objects.h"
#include "Cube.h"
#include "Ground.h"
#include "Axis.h"
#include "Football.h"
#include "Ball.h"

int main()
{
    //初始化部分
    // 创建Render对象，默认800x600窗口
    Render render(2000,1000,100.0f);
    // 初始化（创建窗口）
    if (!render.Init())
    {
        std::cerr << "Render初始化失败！" << std::endl;
        return -1;
    }
    render.SetBackgroundColor(BLACK);

    // 加载自带字体（跨平台，使用相对路径）
    if (!render.LoadFont("assets/fonts/DejaVuSans.ttf", 24))
    {
        std::cerr << "字体加载失败！" << std::endl;
        return -1;
    }

    bool isRunning = true;    // 主循环开关
    SDL_Event event; // 事件对象
    float FPS=60.0;
    float speed =150;
    bool isOrtho = false;  // 当前投影模式：false=透视, true=正交

    // ========== 物体初始化（一行构造，无需手动设顶点和边） ==========

    // 立方体：半边长150, Z范围 500~800
    Cube cube(150, 150, 500, 800);

    // 地面网格：半边长2500, 间距50
    Ground ground(2500, 100);

    // 坐标轴：长度2500
    Axis axis(2500);

    Football football(500);

    Ball ball(300);
    //变化量
    //速度的单位是单位/s
    float dAngle=60.0f;
    float dx=100.0;
    float rotateAngle=45;
    float X=0;

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

        camera.speed=Vector3(0,0,0,0);

        if (keys[SDL_SCANCODE_1])
            speed=100;
        else if (keys[SDL_SCANCODE_2])
            speed=150;
        else if (keys[SDL_SCANCODE_3])
            speed=200;
        else if (keys[SDL_SCANCODE_4])
            speed=300;
        else if (keys[SDL_SCANCODE_5])
            speed=500;
        else if (keys[SDL_SCANCODE_6])
            speed=1000;

        if (keys[SDL_SCANCODE_UP])
            speed+=50;
        else if (keys[SDL_SCANCODE_DOWN])
            speed-=50;

        if (keys[SDL_SCANCODE_W])
            camera.speed.z = speed;
        else if (keys[SDL_SCANCODE_S])
            camera.speed.z = -speed;

        if (keys[SDL_SCANCODE_D])
            camera.speed.x = speed;
        else if (keys[SDL_SCANCODE_A])
            camera.speed.x = -speed;

        if (keys[SDL_SCANCODE_Q])
            camera.speed.y = speed;
        else if (keys[SDL_SCANCODE_E])
            camera.speed.y = -speed;

        if (keys[SDL_SCANCODE_K] && camera.getFront().y>-0.999)//低头
            camera.angleSpeed.x = 60.0;
        else if (keys[SDL_SCANCODE_I] && camera.getFront().y<0.999)//抬头
            camera.angleSpeed.x = -60.0;
        else
            camera.angleSpeed.x = 0;

        if (keys[SDL_SCANCODE_L])//水平右转
            camera.angleSpeed.y = 60.0;
        else if (keys[SDL_SCANCODE_J])//水平左转
            camera.angleSpeed.y = -60.0;
        else
            camera.angleSpeed.y = 0;

        // P键切换投影模式（正交/透视）
        static bool pPressed = false;
        if (keys[SDL_SCANCODE_P])
        {
            if (!pPressed)
            {
                isOrtho = !isOrtho;
                render.SetProjection(isOrtho);
                pPressed = true;
            }
        }
        else
        {
            pPressed = false;
        }

        //物体变化量
        rotateAngle+=dAngle/FPS;
        X+=dx/FPS;

        camera.turn(FPS);
        camera.move(FPS);

        //立方体模型变换矩阵
        Vector3 cubeCenter = cube.getCenter();
        Matrix4 rotateY = Matrix4::RotateY(rotateAngle);
        Matrix4 translateToOrigin = Matrix4::Translate(-cubeCenter.x, -cubeCenter.y, -cubeCenter.z);
        Matrix4 translateBack = Matrix4::Translate(cubeCenter.x, cubeCenter.y, cubeCenter.z);
        cube.modelMatrix = translateBack * rotateY * translateToOrigin;

        //足球模型变换矩阵（放在立方体旁边）
        Matrix4 footballRotate = Matrix4::RotateY(rotateAngle * 0.5f);
        football.modelMatrix = Matrix4::Translate(0, 0, 1000) * footballRotate;

        // 球体旋转动画（放在正前方，保持在视锥体中心避免裁剪伪影）
        Matrix4 ballRotateY = Matrix4::RotateY(rotateAngle * 0.5f);
        Matrix4 ballRotateX = Matrix4::RotateX(rotateAngle * 0.3f);
        ball.modelMatrix = Matrix4::Translate(1000, 0, 0);
        // ========== 绘制 ==========
        render.Clear();

        // 地面网格
        render.Draw3DLines(ground.getRenderEdges(camera.ViewM), YELLOW);

        // 坐标轴（三种颜色分别绘制）
        auto axisEdges = axis.getRenderEdges(camera.ViewM);
        render.Draw3DLine(axisEdges[Axis::X_EDGE].first, axisEdges[Axis::X_EDGE].second, RED);
        render.Draw3DLine(axisEdges[Axis::Y_EDGE].first, axisEdges[Axis::Y_EDGE].second, GREEN);
        render.Draw3DLine(axisEdges[Axis::Z_EDGE].first, axisEdges[Axis::Z_EDGE].second, BLUE);

        // 立方体
        //render.Draw3DLines(cube.getRenderEdges(camera.ViewM), WHITE);

        // 足球
        render.Draw3DLines(football.getRenderEdges(camera.ViewM), WHITE);

        render.Draw3DLines(ball.getRenderEdges(camera.ViewM), WHITE);

        // 显示摄像机坐标（实时变化）
        auto pos = camera.getPosition();
        auto front = camera.getFront();
        render.DrawText(
            std::format("CameraPosition: X={:.1f}  Y={:.1f}  Z={:.1f}", pos.x, pos.y, pos.z),
            10, 10, WHITE);
        render.DrawText(
            std::format("CameraFront: X={:.1f}  Y={:.1f}  Z={:.1f}", front.x, front.y, front.z),
            10, 40, WHITE);
        render.DrawText(std::format("Speed: {:.0f}", speed), 10, 70, GREEN);
        render.DrawText(
            std::format("Projection: {} (P)", isOrtho ? "Orthographic" : "Perspective"),
            10, 100, CYAN);

        render.Present();

        //退出引擎
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT) // 点击窗口关闭按钮
                isRunning = false;
            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                render.HandleResize(event.window.data1, event.window.data2);
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) // 按ESC
                isRunning = false;
        }

        // 控制帧率
        SDL_Delay(1000/FPS);
    }

    std::cout<<"程序退出"<<std::endl;

    return 0;
}
