#pragma once

#include"box2d/box2d.h"
#include"easyx.h"


class camera
{

	float scale = 1.0;//放大缩小比例
	b2Vec2 size;//b2camera size
	b2Vec2 center;
	b2World* world;

public:
	camera() = delete;
	camera(b2World* world);
	void setcenter(b2Vec2 center);
	void setsize(b2Vec2 size,bool reset=false);
	void setscale(float scale);
	bool inarea(b2Vec2 spos);
	bool inarea(b2AABB a);
	float getscale();
	b2Vec2 getcenter();
	b2Vec2 getsize();
	b2Vec2 getSsize();
	b2Vec2 getspos(b2Vec2 pos);
	
};

