#include "camera.h"

camera::camera(b2World* world):world(world),center(0,0)
{
}

void camera::setcenter(b2Vec2 center)
{
	this->center = center;
}


void camera::setsize(b2Vec2 size, bool reset)
{
	if (reset) {
		setscale(scale * this->size.x / size.x );
	}
	this->size = size;
}

void camera::setscale(float scale)
{
	this->scale = scale;
}

bool camera::inarea(b2Vec2 spos)
{
	
	return spos.x <= size.x * scale && spos.x >= 0
		&& spos.y <= size.y * scale && spos.y >= 0;
}

bool camera::inarea(b2AABB a)
{
	b2AABB w;
	auto x = (center.x - size.x / 2);
	auto y = (center.y - size.y / 2);
	w.upperBound = b2Vec2(x, y);
	w.lowerBound = b2Vec2(x + size.x, y + size.y);
	
	return w.Contains(a);
}

float camera::getscale()
{
	return scale;
}

b2Vec2 camera::getcenter()
{
	return b2Vec2(center);
}

b2Vec2 camera::getsize()
{
	return b2Vec2(size);
}

b2Vec2 camera::getSsize()
{
	return b2Vec2(size.x*scale, size.y * scale);
}

b2Vec2 camera::getspos(b2Vec2 b2pos)
{
	auto x = (b2pos.x - (center.x - size.x / 2)) * scale;
	auto y = -(b2pos.y - (center.y + size.y / 2)) * scale;
	return b2Vec2(x,y);
}

