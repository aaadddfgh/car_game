#include "core.h"
#include "loadres.h"
#include"sound.h"
#include "bodys.h"
#include"camera.h"
#include"button.h"
#include <functional>
#include <string>
#include<time.h>
#include <sstream>
static auto world = b2World(b2Vec2(0,-9.8));
Ground1* ground;
Car* car;
camera cam = camera(&world);
bool mute =false;

int start() {
    
    bool stop = false;
    int ret=0;

    auto startbut = button(300, 350, 450, 350,L"开始");
    auto endbut = button(400, 350, 450, 450,L"结束");

    startbut.onmousedown = ([&]() {  stop=true,ret=1; });
    endbut.onmousedown = ([&]() {  stop = true, ret = 0; });

    while (!stop)
    {
        auto msg = getmessage();
        putimage(0, 0, &imgback);
        //cleardevice();
        //setbkcolor(WHITE);
        
        startbut.MouseEve(&msg);
        endbut.MouseEve(&msg);
        startbut.draw();
        //setbkcolor(WHITE);
        endbut.draw();
        putimagePng(400 - imgtitle.getwidth() / 2, 100, &imgtitle);
        FlushBatchDraw();
        Sleep(20);
    }
    return ret;
}

void init() {

    BeginBatchDraw();
    cam.setsize(b2Vec2(20, 15));
    cam.setcenter(b2Vec2(5, 0));
    cam.setscale(800 / 20);
    m_world = &world;
    ground = new Ground1;
    car = new Car;
    
    car->imgcar = &imgcar;
    if(!mute)playspeeds();
    //cam.setcenter(b2Vec2(0, 0));
}

int beforein = 0;

void input()
{
    if (GetKeyState(VK_LEFT) & 0x8000)
        car->getcar()->ApplyAngularImpulse(2, true);

    if (GetKeyState(VK_RIGHT) & 0x8000)
        car->getcar()->ApplyAngularImpulse(-2, true);

    if (GetKeyState(VK_UP)&0x8000) {
        car->getsprA()->SetMotorSpeed(-25);
        car->getsprB()->SetMotorSpeed(-25);
        
        if (beforein !=1&&!mute)
        {
            //playspeed1s();
        }
        beforein = 1;
        return;
    }else{
        if (GetKeyState(VK_DOWN) & 0x8000)
        {
            car->getsprA()->SetMotorSpeed(45);
            car->getsprB()->SetMotorSpeed(45);
        }
        else
        {
            car->getsprA()->SetMotorSpeed(0);
            car->getsprB()->SetMotorSpeed(0);
        }
        
	}
    if (beforein != 0 && !mute) {
        //playspeeds();
    }
    if ((GetKeyState('r') & 0x8000) || (GetKeyState('R') & 0x8000)) {
        world.DestroyBody(car->getcar());
        world.DestroyBody(car->m_wheel1);
        world.DestroyBody(car->m_wheel2);
        world.DestroyBody(ground->body);
        world.DestroyBody(ground->wall);
        delete car;
        delete ground;
        init();

    }
    if ((GetKeyState(VK_ESCAPE) & 0x8000)) {
        PlaySound(NULL, NULL, NULL);
        if (start() == 0)
            exit(0);
        else {
            world.DestroyBody(car->getcar());
            world.DestroyBody(car->m_wheel1);
            world.DestroyBody(car->m_wheel2);
            world.DestroyBody(ground->body);
            world.DestroyBody(ground->wall);
            delete car;
            delete ground;
            init();
        }
    }
    beforein = 0;
}

void update()
{
    cam.setcenter(car->getpos()+b2Vec2(0,2));
	m_world->Step(float(1)/60,10,4);
    if (car->getcar()->GetPosition().x > 288) {
        auto endbut = button(450, 350, 450, 500, L"结束");
        car->getsprA()->SetMotorSpeed(0);
        car->getsprB()->SetMotorSpeed(0);

        bool stop = false;

        endbut.onmousedown = ([&]() {  
            stop = true; 
            PlaySound(NULL, NULL, NULL);
            });
        while (!stop)
        {
            
            m_world->Step(float(1) / 60, 10, 4);
            //auto t1 = clock();
            putimage(0, 0, &imgback);
            auto speed = pow(car->getcar()->GetLinearVelocity().x, 2) + pow(car->getcar()->GetLinearVelocity().y, 2);
            auto newXsize = -4000.0 / (speed + 200) + 40;
            auto newYsize = 15 * newXsize / 20;
            cam.setsize(b2Vec2(newXsize, newYsize), true);
            ground->draw(cam);
            car->draw(cam);
            
            putimagePng(400 - imgwin.getwidth() / 2, 50, &imgwin);
            //cleardevice();
            //setbkcolor(WHITE);
            //startbut.draw();
            //setbkcolor(WHITE);
            //if (msg.lbutton) {
            //    OutputDebugStringA("left");
            //    std::string str;
            //    std::stringstream ss;
            //    //int a, b;
            //    //cin >> x;
            //    ss << msg.x<<' '<<msg.y;
            //    str=ss.str();
            //    OutputDebugStringA((LPCSTR)str.c_str());
            //    //OutputDebugStringA();
            //}
            auto msg = getmessage();
            endbut.MouseEve(&msg);
            endbut.draw();
            
            FlushBatchDraw();
            Sleep(10);
            //auto t2 = clock();
            //int t = t2 - t1;
            //if (100 / 6 - t > 0)
               // Sleep(1000 / 60 - t);
        }
        world.DestroyBody(car->getcar());
        world.DestroyBody(car->m_wheel1);
        world.DestroyBody(car->m_wheel2);
        world.DestroyBody(ground->body);
        world.DestroyBody(ground->wall);
        delete car;
        delete ground;
        switch (start()) {
        case 0:
            exit(0);
        case 1:
            break;
        }
        init();
     }
}

void draw()
{
    //cam.setcenter(b2Vec2(ittt, 0));
    putimage(0, 0, &imgback);
    auto speed = pow(car->getcar()->GetLinearVelocity().x, 2) 
        + pow(car->getcar()->GetLinearVelocity().y, 2);
    auto newXsize = -4000.0 / (speed + 200) + 40;
    auto newYsize = 15 * newXsize / 20;
    cam.setsize(b2Vec2(newXsize, newYsize),true);
    ground->draw(cam);
    car->draw(cam);
    //putimage(-50, 0, &imgcar, SRCPAINT);
    //putimage(-10, 0, &imgcar);
    //putimage(0, 0, &masktree);
    FlushBatchDraw();
}
