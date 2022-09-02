// easyxxx.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include <iostream>
#include"box2d/box2d.h"
#include "test.h"
#ifdef _DEBUG
#pragma comment(lib,"box2d.lib")
#else
#pragma comment(lib,"box2drel.lib")
#endif // DEBUG


#include<time.h>

// Bridge
class car {
public:
    b2World* m_world;
    b2Body* m_car;
    b2Body* m_wheel1;
    b2Body* m_wheel2;

    float m_speed;
    b2WheelJoint* m_spring1;
    b2WheelJoint* m_spring2;

    car() {
        b2Body* ground = NULL;
        {
            b2BodyDef bd;
            ground = m_world->CreateBody(&bd);

            b2EdgeShape shape;

            b2FixtureDef fd;
            fd.shape = &shape;
            fd.density = 0.0f;
            fd.friction = 0.6f;

            shape.SetTwoSided(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
            ground->CreateFixture(&fd);

            float hs[10] = { 0.25f, 1.0f, 4.0f, 0.0f, 0.0f, -1.0f, -2.0f, -2.0f, -1.25f, 0.0f };

            float x = 20.0f, y1 = 0.0f, dx = 5.0f;

            for (int32 i = 0; i < 10; ++i)
            {
                float y2 = hs[i];
                shape.SetTwoSided(b2Vec2(x, y1), b2Vec2(x + dx, y2));
                ground->CreateFixture(&fd);
                y1 = y2;
                x += dx;
            }

            for (int32 i = 0; i < 10; ++i)
            {
                float y2 = hs[i];
                shape.SetTwoSided(b2Vec2(x, y1), b2Vec2(x + dx, y2));
                ground->CreateFixture(&fd);
                y1 = y2;
                x += dx;
            }

            shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
            ground->CreateFixture(&fd);

            x += 80.0f;
            shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
            ground->CreateFixture(&fd);

            x += 40.0f;
            shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 10.0f, 5.0f));
            ground->CreateFixture(&fd);

            x += 20.0f;
            shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
            ground->CreateFixture(&fd);

            x += 40.0f;
            shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x, 20.0f));
            ground->CreateFixture(&fd);
        }

        {
        int32 N = 20;
        b2PolygonShape shape;
        shape.SetAsBox(1.0f, 0.125f);

        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 1.0f;
        fd.friction = 0.6f;

        b2RevoluteJointDef jd;

        b2Body* prevBody = ground;
        for (int32 i = 0; i < N; ++i)
        {
            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(161.0f + 2.0f * i, -0.125f);
            b2Body* body = m_world->CreateBody(&bd);
            body->CreateFixture(&fd);

            b2Vec2 anchor(160.0f + 2.0f * i, -0.125f);
            jd.Initialize(prevBody, body, anchor);
            m_world->CreateJoint(&jd);

            prevBody = body;
        }

        b2Vec2 anchor(160.0f + 2.0f * N, -0.125f);
        jd.Initialize(prevBody, ground, anchor);
        m_world->CreateJoint(&jd);
    }
        // Boxes
        {
            b2PolygonShape box;
            box.SetAsBox(0.5f, 0.5f);

            b2Body* body = NULL;
            b2BodyDef bd;
            bd.type = b2_dynamicBody;

            bd.position.Set(230.0f, 0.5f);
            body = m_world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);

            bd.position.Set(230.0f, 1.5f);
            body = m_world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);

            bd.position.Set(230.0f, 2.5f);
            body = m_world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);

            bd.position.Set(230.0f, 3.5f);
            body = m_world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);

            bd.position.Set(230.0f, 4.5f);
            body = m_world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);
        }

        // Car
        {
            b2PolygonShape chassis;
            b2Vec2 vertices[8];
            vertices[0].Set(-1.5f, -0.5f);
            vertices[1].Set(1.5f, -0.5f);
            vertices[2].Set(1.5f, 0.0f);
            vertices[3].Set(0.0f, 0.9f);
            vertices[4].Set(-1.15f, 0.9f);
            vertices[5].Set(-1.5f, 0.2f);
            chassis.Set(vertices, 6);

            b2CircleShape circle;
            circle.m_radius = 0.4f;

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 1.0f);
            m_car = m_world->CreateBody(&bd);
            m_car->CreateFixture(&chassis, 1.0f);

            b2FixtureDef fd;
            fd.shape = &circle;
            fd.density = 1.0f;
            fd.friction = 0.9f;

            bd.position.Set(-1.0f, 0.35f);
            m_wheel1 = m_world->CreateBody(&bd);
            m_wheel1->CreateFixture(&fd);

            bd.position.Set(1.0f, 0.4f);
            m_wheel2 = m_world->CreateBody(&bd);
            m_wheel2->CreateFixture(&fd);

            b2WheelJointDef jd;
            b2Vec2 axis(0.0f, 1.0f);

            float mass1 = m_wheel1->GetMass();
            float mass2 = m_wheel2->GetMass();

            float hertz = 4.0f;
            float dampingRatio = 0.7f;
            float omega = 2.0f * b2_pi * hertz;

            jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
            jd.motorSpeed = 0.0f;
            jd.maxMotorTorque = 20.0f;
            jd.enableMotor = true;
            jd.stiffness = mass1 * omega * omega;
            jd.damping = 2.0f * mass1 * dampingRatio * omega;
            jd.lowerTranslation = -0.25f;
            jd.upperTranslation = 0.25f;
            jd.enableLimit = true;
            m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&jd);

            jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
            jd.motorSpeed = 0.0f;
            jd.maxMotorTorque = 10.0f;
            jd.enableMotor = false;
            jd.stiffness = mass2 * omega * omega;
            jd.damping = 2.0f * mass2 * dampingRatio * omega;
            jd.lowerTranslation = -0.25f;
            jd.upperTranslation = 0.25f;
            jd.enableLimit = true;
            m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&jd);
        }
    }
};


int ccmain()
{
    
    auto world = new b2World(b2Vec2(0, -9.8));
    //world.SetContinuousPhysics(true);
    
    //b2ContactListener cc;
    //b2Contact();
    //cc.BeginContact = []() {};
    
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(0.0f, 90.0f);
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(500.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);


    auto bodyDef=new b2BodyDef();
    bodyDef->type = b2_dynamicBody;
    bodyDef->position.Set(200.0f, 335.0f);
    bodyDef->bullet = true;
    auto body = world->CreateBody(bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(5.0f, 5.0f);
    b2FixtureDef* fixtureDef=new b2FixtureDef();
    fixtureDef->shape = &dynamicBox;
    fixtureDef->density = 1.0f;
    fixtureDef->friction = 0.3f;
    
    fixtureDef->restitution = 0.0f;
    fixtureDef->restitutionThreshold = 0.0f;
    
    body->CreateFixture(fixtureDef);
    fixtureDef = new b2FixtureDef();
    fixtureDef->shape = &dynamicBox;
    fixtureDef->density = 1.0f;
    fixtureDef->friction = 0.3f;

    fixtureDef->restitution = 0.0f;
    fixtureDef->restitutionThreshold = 0.0f;
    //body->GetMass();

    b2PolygonShape boxShape;
    boxShape.SetAsBox(25, 25);

    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_dynamicBody; //this will be a kinematic body
    myBodyDef.position.Set(200.0f, 300.0f); // start from left side, slightly above the static body
    b2Body* kinematicBody = world->CreateBody(&myBodyDef); //add body to world
    kinematicBody->CreateFixture(&boxFixtureDef); //add fixture to body
    //kinematicBody->GetMass();
    //kinematicBody->SetLinearVelocity(b2Vec2(1, 0)); //move right 1 unit per second
     //1 turn per second counter-clockwise
    //kinematicBody->SetBullet(true);

    auto* JointDef=new b2WheelJointDef();
    JointDef->Initialize(kinematicBody, groundBody, 
        kinematicBody->GetPosition(),
        b2Vec2(0,1));
    JointDef->stiffness = 800.0f;
    JointDef->damping = 220.5f;
    /*b2RevoluteJointDef revoluteJointDef;
    revoluteJointDef.Initialize( kinematicBody, groundBody, b2Vec2(200.0f, 300.0f));
    revoluteJointDef.enableMotor = true;
    revoluteJointDef.maxMotorTorque = 25000;
    revoluteJointDef.motorSpeed = 3 *  0.0174532925199432957f;
    revoluteJointDef.collideConnected = false;*/
    //revoluteJointDef.localAnchorA = b2Vec2(0.0f, 0.0f);
    //revoluteJointDef.localAnchorB = b2Vec2(0.0f, 0.0f);
    //revoluteJointDef.enableLimit(true);

    world->CreateJoint(JointDef);

    initgraph(500, 500);

    IMAGE src, mask;

    loadimage(&src, L"1.png");
    loadimage(&mask, L"2.png");

    //fillrectangle(0, 0, 500, 100);
    //rotateimage();
    while (true)
    {
        auto t1 = clock();
        
        world->Step(1.0f / 60.0f, 8, 3);
        //kinematicBody->SetAngularVelocity(3 * 0.0174532925199432957f);
        auto ti=IMAGE(500, 500);
        SetWorkingImage(&ti);
        fillrectangle(0, 0, 500, 100);
       
        b2Vec2 position = body -> GetPosition();
        auto ttt = dynamic_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape());
        
        POINT pts[4];

        for (size_t i = 0; i < 4; i++)
        {
            auto tp= body->GetWorldPoint(ttt->m_vertices[i]);
            pts[i].x = tp.x;
            pts[i].y = tp.y;
        }

        fillpolygon(pts, 4);

        //transparentimage()
        setfillcolor(GREEN);
        fillrectangle(300, 300, 500, 500);
        setfillcolor(WHITE);

        position = kinematicBody->GetPosition();
        auto angle = kinematicBody->GetAngle();
        
        IMAGE a, b;
        rotateimage(&a, &src, -angle, BLACK, true);
        rotateimage(&b, &mask, -angle, WHITE, true);
        auto pt = position.y - a.getheight() / 2.0;
        auto pl = position.x - a.getwidth() / 2.0;
        transparentimage(pl, pt, &a, &b);


        //fillrectangle(position.x - 5, position.y - 5, position.x + 5, position.y + 5);

        SetWorkingImage();
        putimage(0, 0, &ti);
        auto t2 = clock();
        float t = t2 - t1;
        if (100 / 6 - t > 0)
            Sleep(1000 / 60 - t);
    }
    /*initgraph(1000, 1000);
    auto i = IMAGE();
    loadimage(&i, L"1.gif");
    auto ip = new IMAGE();
    rotateimage(ip, &i, acos(-1)/6);
    putimage(0, 0, ip);
    system("pause");*/
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
