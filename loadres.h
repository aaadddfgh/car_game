#pragma once

#include "easyx.h"

IMAGE imgcar;
IMAGE maskcar;

IMAGE imgwheel;
IMAGE maskwheel;

IMAGE imgtree;
IMAGE masktree;

IMAGE imgend;

IMAGE imgback;
IMAGE imgtitle;
IMAGE imgwin;

void imginit() {
	loadimage(&imgback, L"background.jpg");
	loadimage(&imgcar, L"car.bmp");
	loadimage(&maskcar, L"maskcar1.bmp");
	loadimage(&imgwheel, L"wheel1.bmp");
	loadimage(&maskwheel, L"maskwheel.bmp");
	loadimage(&imgtitle, L"title.png");
	loadimage(&imgwin, L"imgwin.png");
	loadimage(&imgtree, L"tree.jpg");
	loadimage(&masktree, L"masktree.jpg");
	loadimage(&imgend,L"end.jpg");
}
