#include "sound.h"
#include<Windows.h>
#include<mmsystem.h>
#include <cstddef>
#pragma comment(lib,"winmm.lib")
void playspeeds()
{
	PlaySound(L"back.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
}
void playspeed1s() {
	//PlaySound(L"speed.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void playBGM();