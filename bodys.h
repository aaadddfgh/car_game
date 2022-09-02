#pragma once
#include"box2d/box2d.h"
#include "easyx.h"
#include "camera.h"
//#include<array>
#include <vector>

void putimagePng(int img_x, int img_y, IMAGE* pSrcImg)
{
    DWORD* dst = GetImageBuffer();
    DWORD* src = GetImageBuffer(pSrcImg);
    int src_width = pSrcImg->getwidth();
    int src_height = pSrcImg->getheight();
    int dst_width = getwidth();
    int dst_height = getheight();

    int im_width, im_height;
    if (img_x + src_width > dst_width)
        im_width = dst_width - img_x;
    else
        im_width = src_width;
    if (img_y + src_height > dst_height)
        im_height = dst_height - img_y;
    else
        im_height = src_height;

    if (img_x < 0)
    {
        src = src - img_x;
        im_width = im_width + img_x;
        img_x = 0;
    }
    if (img_y < 0)
    {
        src = src - src_width * img_y;
        im_height = im_height + img_y;
        img_y = 0;
    }
    dst = dst + dst_width * img_y + img_x;

    for (int i = 0; i < im_height; i++)
    {
        for (int j = 0; j < im_width; j++)
        {
            int src_r = ((src[j] & 0xff0000) >> 16);
            int src_g = ((src[j] & 0xff00) >> 8);
            int src_b = src[j] & 0xff;
            int src_a = ((src[j] & 0xff000000) >> 24);
            int dst_r = ((dst[j] & 0xff0000) >> 16);
            int dst_g = ((dst[j] & 0xff00) >> 8);
            int dst_b = dst[j] & 0xff;
            dst[j] = ((src_r + dst_r * (255 - src_a) / 255) << 16)
                | ((src_g + dst_g * (255 - src_a) / 255) << 8)
                | (src_b + dst_b * (255 - src_a) / 255);
        }
        dst = dst + dst_width;
        src = src + src_width;
    }
}

void transparentimage(int x, int y, IMAGE* srcimg, IMAGE* maskimg)
{
    putimage(x, y, maskimg, SRCAND);
    putimage(x, y, srcimg, SRCPAINT);
}

/**
    *函数名:getImage(IMAGE* pImg,int x0,int y0,int width,int height)
    *参数说明:pImg表示原图片指针，x0,y0表示取图的起始坐标，width和height是取图的长宽
    *函数功能:从pImg中截取一个子图
    *返回新的图片（子图）
    */
IMAGE getImage(IMAGE* pImg, int x0, int y0, int width, int height) {
    //判断width和height是否超出尺寸并且进行调整
    if (width > pImg->getwidth() - x0 || height > pImg->getheight() - y0) {
        width = pImg->getwidth() - x0;
        height = pImg->getheight() - y0;
    }


    //将要被返回的图片
    IMAGE newImg(width, height);


    //分别对原图像和目标图像获取指针
    DWORD* oldDr = GetImageBuffer(pImg);
    DWORD* newDr = GetImageBuffer(&newImg);

    //赋值
    for (short i = 0; i < height; i++) {
        for (short j = 0; j < width; j++) {
            newDr[i * width + j] = oldDr[y0 * pImg->getwidth() + x0 + i * pImg->getwidth() + j];
        }
    }


    return newImg;
}

/**
*函数名:zoomImage(IMAGE* pImg,int width，int height)
*参数说明:pImg是原图指针，width1和height1是目标图片的尺寸。
*函数功能:将图片进行缩放，返回目标图片 可以自定义长与宽，也可以只给长自动计算宽
*返回目标图片
*/
IMAGE zoomImage(IMAGE* pImg, int newWidth, int newHeight = 0) {
    //防止越界
    if (newWidth < 0 || newHeight < 0) {
        newWidth = pImg->getwidth();
        newHeight = pImg->getheight();
    }
    //当参数只有一个时按比例自动缩放
    if (newHeight == 0) {
        //先*再/。不然当目标图片小于原图时会出错
        newHeight = newWidth * pImg->getheight() / pImg->getwidth();
    }
    //获取需要进行缩放的图片
    IMAGE newImg(newWidth, newHeight);
    //分别对原图像和目标图像获取指针
    DWORD* oldDr = GetImageBuffer(pImg);
    DWORD* newDr = GetImageBuffer(&newImg);
    //赋值 使用最邻近插值算法
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            int t = i * newWidth + j;
            int xt = j * pImg->getwidth() / newWidth;
            int yt = i * pImg->getheight() / newHeight;
            newDr[i * newWidth + j] = oldDr[xt + yt * pImg->getwidth()];
            byte r = (GetRValue(oldDr[xt + yt * pImg->getwidth()]));
            byte g = (GetGValue(oldDr[xt + yt * pImg->getwidth()]));
            byte b = (GetBValue(oldDr[xt + yt * pImg->getwidth()]));
            /*byte a = (((oldDr[xt + yt * pImg->getwidth()] & 0xff000000) >> 24) +
                (((oldDr[xt + yt * pImg->getwidth() + 1]) & 0xff000000) >> 24) +
                (((oldDr[xt + (yt + 1) * pImg->getwidth()]) & 0xff000000) >> 24) +
                (((oldDr[xt + (yt + 1) * pImg->getwidth() + 1]) & 0xff000000) >> 24)) / 4;*/
            newDr[i * newWidth + j] = RGB(r, g, b);
        }
    }

    return newImg;
}


/**
*函数名:zoomImage(IMAGE* pImg,int x0,int y0,int width0,int height0,int width1,int height1)
*参数说明:重载zoomImage函数，x0,hy0是获取原图的地址坐标
*width0与height0是获取尺寸。width1和height1是目标图片尺寸
*函数功能:将图片进行缩放，返回目标图片 可以自定义长与宽，也可以只给长自动计算宽
*重载添加的功能： 可以从原图片中选取子图进行伸缩
*返回目标图片
*/
IMAGE zoomImage(IMAGE* pImg, int x0, int y0, int width0, int height0, int width1, int height1 = 0) {
    IMAGE img = getImage(pImg, x0, y0, width0, height0);
    return zoomImage(&img, width1, height1);
}

b2World* m_world;

class  Ground1
{
public:
	 Ground1();
	~ Ground1();
    void draw(camera& cam);
    b2Body* body;
    b2Body* wall;
private:
	
    std::vector<b2Vec2> treepos;
};

 Ground1:: Ground1()
{
     b2BodyDef bd;
     b2BodyDef wbd;
     body = m_world->CreateBody(&bd);
     wall= m_world->CreateBody(&wbd);
     b2EdgeShape shape,wshape;

     b2FixtureDef fd;
     fd.shape = &shape;
     fd.density = 0.0f;
     fd.friction = 0.6f;

     b2FixtureDef wfd;
     wfd.shape = &wshape;
     wfd.density = 0.0f;
     wfd.friction = 0.6f;

     wshape.SetTwoSided(b2Vec2(-20.0f, 0.0f), b2Vec2(-20.0f, 10.0f));
     wall->CreateFixture(&wfd);
     shape.SetTwoSided(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
     body->CreateFixture(&fd);

     float hs[10] = { 0.25f, 1.0f, 4.0f, 0.0f, 0.0f, -1.0f, -2.0f, -2.0f, -1.25f, 0.0f };

     float x = 20.0f, y1 = 0.0f, dx = 5.0f;

     for (int32 i = 0; i < 10; ++i)
     {
         float y2 = hs[i];
         shape.SetTwoSided(b2Vec2(x, y1), b2Vec2(x + dx, y2));
         body->CreateFixture(&fd);
         y1 = y2;
         x += dx;
     }

     for (int32 i = 0; i < 10; ++i)
     {
         float y2 = hs[i];
         shape.SetTwoSided(b2Vec2(x, y1), b2Vec2(x + dx, y2));
         body->CreateFixture(&fd);
         y1 = y2;
         x += dx;
     }
     
     shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 80.0f, 0.0f));
     body->CreateFixture(&fd);
     treepos.push_back( b2Vec2(x + 20.0f, 0.0f));
     treepos.push_back(b2Vec2(x + 40.0f, 0.0f));
     treepos.push_back(b2Vec2(x + 60.0f, 0.0f));
     x += 80.0f;
     shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
     body->CreateFixture(&fd);

     x += 40.0f;
     shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 10.0f, 5.0f));
     body->CreateFixture(&fd);

     x += 20.0f;
     shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
     body->CreateFixture(&fd);

     x += 40.0f;
     wshape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x, 20.0f));
     wall->CreateFixture(&wfd);
}

 Ground1::~ Ground1()
{
     ;
}

 inline void Ground1::draw(camera& cam)
 {

     for (auto&& wpos : treepos)
     {
         auto pos = cam.getspos(wpos);
         //fillcircle(pos.x, pos.y, 8);
         if (cam.inarea(pos)|| cam.inarea(pos + b2Vec2(imgtree.getwidth(), 0)))
         {
             auto tr = zoomImage(&imgtree,
                 int(imgtree.getwidth() * 4.0 / 150 * cam.getscale()),
                 int(imgtree.getheight() * 4.0 / 150 * cam.getscale())
             );
             auto trm = zoomImage(&masktree,
                 int(masktree.getwidth() * 4.0 / 150 * cam.getscale()),
                 int(masktree.getheight() * 4.0 / 150 * cam.getscale())
             );
             //putimagePng(cam.getspos(treepos).x, cam.getspos(treepos).y-tr.getheight(), &tr);
             //putimage(0,0,)
             transparentimage(pos.x, pos.y - tr.getheight(), &tr, &trm);
         }
     }
     setlinestyle(PS_SOLID);
     setfillcolor(BGR(0x522A04));
     setlinecolor(BGR(0x522A04));
     auto ll = body->GetFixtureList();
     b2Vec2 pos1, pos2;
     do
     {
         b2EdgeShape* ss= (b2EdgeShape*)ll->GetShape();
         pos1 = cam.getspos(ss->m_vertex1);
         pos2 = cam.getspos(ss->m_vertex2);
         /*if (!cam.inarea(pos1) && !cam.inarea(pos2)) {
             ll = ll->GetNext();
             continue;
         }*/
         POINT pts[4] = { POINT({int(pos1.x),int(pos1.y)}),
             POINT({int(pos2.x),int(pos2.y)}),
             POINT({int(pos2.x),int(cam.getSsize().y)}),
             POINT({ int(pos1.x),int(cam.getSsize().y) })
         };
         fillpolygon(pts,4);
         ll=ll->GetNext();
     } while ( ll != nullptr);
     {
         b2Vec2 endp = { 288.0, 0.0 };
         auto pos = cam.getspos(endp);
         //fillcircle(pos.x, pos.y, 8);
         if (cam.inarea(pos))
         {
             auto endi = zoomImage(&imgend,
                 int(imgend.getwidth() * 2.0 / 113 * cam.getscale())
                 //int(imgtree.getheight() * 4.0 / 150 * cam.getscale())
             );
             putimage(pos.x, pos.y, &endi);
             /*auto trm = zoomImage(&masktree,
                 int(masktree.getwidth() * 4.0 / 150 * cam.getscale()),
                 int(masktree.getheight() * 4.0 / 150 * cam.getscale())
             );*/
             //putimagePng(cam.getspos(treepos).x, cam.getspos(treepos).y-tr.getheight(), &tr);
             //putimage(0,0,)
             //transparentimage(pos.x, pos.y - tr.getheight(), &tr, &trm);
         }
     }
     //else {
     //    //auto pos = cam.getspos(treepos);
     //    auto tr = zoomImage(&imgtree,
     //        int(imgtree.getwidth() * 4.0 / 750 * cam.getscale()),
     //        int(imgtree.getheight() * 4.0 / 750 * cam.getscale())
     //    );
     //    auto trm = zoomImage(&masktree,
     //        int(imgtree.getwidth() * 4.0 / 750 * cam.getscale()),
     //        int(imgtree.getheight() * 4.0 / 750 * cam.getscale())
     //    );
     //    transparentimage(3, 50, &tr, &trm);
     //    putimage(80, 0, & trm);
     //    putimage(500, 0, &tr);
     //}
 }

 class  Ground2
 {
 public:
     Ground2();
     ~Ground2()=default;
     void draw(camera& cam);
 private:
     b2Body* body;
     b2Body* wall;
 };

 Ground2::Ground2()
 {
 }

 inline void Ground2::draw(camera& cam)
 {

 }

 class Car
 {
 public:
     Car();
     ~Car();
     void draw(camera& cam);
     b2Vec2 getpos();
     IMAGE* imgcar;
     inline b2Body* getcar() { return m_car; }
     inline b2WheelJoint* getsprA(){ return m_spring1; }
     inline b2WheelJoint* getsprB() { return m_spring2; }
     b2Body* m_wheel1;
     b2Body* m_wheel2;
 private:
     b2Body* m_car;
     b2PolygonShape* fix_car;

     float radius;
     

     float m_speed;
     b2WheelJoint* m_spring1;
     b2WheelJoint* m_spring2;

    
     
 };

 Car::Car()
 {
     
     //主车体
     
         b2PolygonShape* chassis = new b2PolygonShape;
         b2Vec2 vertices[8];
         vertices[0].Set(-2.189, 0.178f);
         vertices[1].Set(-2.189, 1.867f);
         vertices[2].Set(2.422 - 2.189f, 1.867f);
         vertices[3].Set(3.089 - 2.189f, 1.156f);
         vertices[4].Set(4.378 - 2.189f, 1.156f);
         vertices[5].Set(4.378 - 2.189f, 0.178f);
         chassis->Set(vertices, 6);


         b2BodyDef bd;
         bd.type = b2_dynamicBody;
         bd.position.Set(0.0, 4.0f);
         m_car = m_world->CreateBody(&bd);
         m_car->CreateFixture(chassis, 1.0f);
         fix_car = chassis;
         
     
     //轮子
     
         
         b2CircleShape circle;
         radius = 0.6f;
         circle.m_radius = radius;

         b2FixtureDef fd;
         fd.shape = &circle;
         fd.density = 1.0f;
         fd.friction = 0.9f;

         bd.position.Set(0.783f - 2.189f, m_car->GetPosition().y-radius + 0.45);
         m_wheel1 = m_world->CreateBody(&bd);
         m_wheel1->CreateFixture(&fd);

         bd.position.Set(3.413f - 2.189f, m_car->GetPosition().y - radius+0.45);
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
         jd.enableMotor = true;
         jd.stiffness = mass2 * omega * omega;
         jd.damping = 2.0f * mass2 * dampingRatio * omega;
         jd.lowerTranslation = -0.25f;
         jd.upperTranslation = 0.25f;
         jd.enableLimit = true;
         m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&jd);
     

         //m_car->ApplyAngularImpulse(3, false);
         
 }

 Car::~Car()
 {
 }

 inline void Car::draw(camera& cam)
 {
     constexpr float carscale = 2.0 / 92;

     IMAGE* scr = imgcar;
     
     IMAGE ss=zoomImage(scr, 
         int(scr->getwidth() * carscale * cam.getscale()),
         int(scr->getheight() * carscale * cam.getscale()));
     IMAGE ss2 = zoomImage(&maskcar,
         int(scr->getwidth() * carscale * cam.getscale()),
         int(scr->getheight() * carscale * cam.getscale()));
     IMAGE TT,mm;
     rotateimage(&TT, &ss, m_car->GetAngle(), 0x00, true);
     rotateimage(&mm, &ss2, m_car->GetAngle(), WHITE , true);
     //mm.Resize(mm.getwidth() + 2, mm.getheight() + 2);
     //putimage(0, 0, &ss2);
     auto ang = m_car->GetAngle();
     b2Vec2 rot(0, 1.1);
     auto pos = cam.getspos(m_car->GetPosition() + b2Vec2(
         rot.x * cos(ang) - sin(ang) * rot.y,
         rot.x * sin(ang) + cos(ang) * rot.y
         ));
     pos -= b2Vec2(float(TT.getwidth())/2, float(TT.getheight())/2);

     transparentimage(pos.x, pos.y, &TT,&mm);

     ang = m_wheel1->GetAngle();
     ss= zoomImage(&imgwheel,
         int(radius*2 * cam.getscale()));
     ss2 = zoomImage(&maskwheel,
         int(radius * 2 * cam.getscale()));
     rotateimage(&TT, &ss, ang, 0x00, true);
     rotateimage(&mm, &ss2, ang, WHITE, true);
     pos = cam.getspos(m_wheel1->GetPosition());
     setlinestyle(PS_SOLID, 0.2 * cam.getscale());
     setlinecolor(0xc0c0c0);
     rot= cam.getspos(m_car->GetWorldPoint(b2Vec2(0.783f - 2.189f, 0.45f)));
     line(pos.x, pos.y,
         rot.x, rot.y);
     pos -= b2Vec2(float(TT.getwidth()) / 2, float(TT.getheight()) / 2);     
     transparentimage(pos.x, pos.y, &TT, &mm);

     ang = m_wheel2->GetAngle();
     rotateimage(&TT, &ss, ang, 0x00, true);
     rotateimage(&mm, &ss2, ang, WHITE, true);
     pos = cam.getspos(m_wheel2->GetPosition());
     rot = cam.getspos(m_car->GetWorldPoint(b2Vec2(3.413f - 2.189f, 0.45f)));
     line(pos.x, pos.y,
         rot.x, rot.y);
     pos -= b2Vec2(float(TT.getwidth()) / 2, float(TT.getheight()) / 2);
     transparentimage(pos.x, pos.y, &TT, &mm);

     //debug
     /*setlinecolor(WHITE);
     auto ttttt = ((b2CircleShape*)(m_wheel1->GetFixtureList()->GetShape()))->m_radius;
     circle(pos.x, pos.y, int(ttttt * cam.getscale()));*/
     /*{
         auto ll = fix_car->m_vertices;

         for (size_t i = 0; i < 5; i++)
         {
             auto a= m_car->GetWorldPoint(ll[i]);
             auto b = m_car->GetWorldPoint(ll[(i+1)%5]);
             a = cam.getspos(a);
             b = cam.getspos(b);
             line(a.x, a.y, b.x, b.y);
         }
     }*/
 }

 inline b2Vec2 Car::getpos()
 {
     return b2Vec2(m_car->GetPosition());
 }
