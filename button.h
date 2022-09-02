#pragma once
#include"easyx.h"
#include<functional>

struct ClassMouseState
{
	bool mouseon;
	bool mousedown;
	bool nowdown;
};
class button
{
public:
	RECT re;
	struct ClassMouseState state;//��ǰ״̬
	struct ClassMouseState lastState;
	bool visible;
	bool click;
	bool name;
	bool roundCorner;
	LPTSTR text;
	COLORREF onColor;//����ƶ��ڰ�ť�ؼ�����ɫ����
	COLORREF outColor;//����ƿ�����ɫ����
	COLORREF downColor;//��갴�º����ɫ
	COLORREF forecolor;//ǰ��ɫ
	std::function<void()> onmousedown;
	//void(*onmousedown)()=nullptr;

	void MouseEve(ExMessage* mouse)
	{
		
		if (mouse->lbutton)
		{
			//std::string s;
			//s += itoa(mouse->x);
			//OutputDebugStringA();
			if (state.mousedown == true
				&& mouse->x<re.right
				&& mouse->y<re.bottom
				&& mouse->x>re.left
				&& mouse->y>re.left) {
				click = true;
				
			}
			state.mousedown = false;
			state.nowdown = false;
		}
		if (mouse->x<re.right 
			&& mouse->y<re.bottom 
			&& mouse->x>re.left 
			&& mouse->y>re.top)
		{
			state.mouseon = true;
			if (mouse->lbutton || state.mousedown)
			{
				state.nowdown = true;
				state.mousedown = true;
				if (bool(onmousedown))
				{
					onmousedown();
				}
			}
		}
		else
		{
			state.mouseon = false;
			state.nowdown = false;
		}
	}
	void refresh()
	{
		if (visible == false)
			return;
		//if ((state.mouseon == lastState.mouseon && state.mousedown == lastState.mousedown && state.nowdown == lastState.nowdown))
		//	return;//������ֹһֱˢ�� ����ϴθ��µ�״̬�ͱ���һ���Ͳ�ˢ�°�ť
		lastState = state;
		if (state.nowdown && state.mousedown)
		{
			setbkcolor(downColor);
			setfillcolor(downColor);
			settextcolor(forecolor);
		}
		if ((!state.nowdown) && state.mouseon)
		{
			setbkcolor(onColor);
			setfillcolor(onColor);
			settextcolor(forecolor);
		}
		if (state.mouseon == false)
		{
			setbkcolor(outColor);
			setfillcolor(outColor);
			settextcolor(forecolor);
		}
		else
		{
			if (!state.mousedown)
			{
				setbkcolor(onColor);
				setfillcolor(onColor);
				settextcolor(forecolor);
			}
		}
		fillroundrect(re.left, re.top, re.right, re.bottom, 10, 10);
		drawtext(text, &re, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	}
	void draw(){
		if (visible == false)
			return;
		if (state.nowdown && state.mousedown)
		{
			setbkcolor(downColor);
			setfillcolor(downColor);
			settextcolor(forecolor);
		}
		if ((!state.nowdown) && state.mouseon)
		{
			setbkcolor(onColor);
			setfillcolor(onColor);
			settextcolor(forecolor);
		}
		if (state.mouseon == false)
		{
			setbkcolor(outColor);
			setfillcolor(outColor);
			settextcolor(forecolor);
		}
		else
		{
			if (!state.mousedown)
			{
				setbkcolor(onColor);
				setfillcolor(onColor);
				settextcolor(forecolor);
			}
		}
		fillroundrect(re.left, re.top, re.right, re.bottom, 10, 10);
		drawtext(text, &re, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	}
	
	button(int top, int left, int right, int bottom, const wchar_t* text);

};