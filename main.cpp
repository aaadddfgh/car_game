
//#include "test.h"
#include"easyx.h"
#include"box2d/box2d.h"
#include<time.h>
#include"core.h"
#ifdef _DEBUG
#pragma comment(lib,"box2d.lib")
#else
#pragma comment(lib,"box2drel.lib")
#endif

int main() {
	initgraph(800, 600);
	imginit();
	BeginBatchDraw();

	switch (start()) {
		case 0:
			return 0;
		case 1:
			break;
}
	init();
	while (true)
	{
		auto t1 = clock();
		input();
		update();
		draw();
		
		auto t2 = clock();
		int t = t2 - t1;
		if (100 / 6 - t > 0)
			Sleep(1000 / 60 - t);
	}
	EndBatchDraw();
	closegraph();
	return 0;
}