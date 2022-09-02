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
	struct ClassMouseState state;//当前状态
	struct ClassMouseState lastState;
	bool visible;
	bool click;
	bool name;
	bool roundCorner;
	LPTSTR text;
	COLORREF onColor;//鼠标移动在按钮控件的颜色设置
	COLORREF outColor;//鼠标移开的颜色设置
	COLORREF downColor;//鼠标按下后的颜色
	COLORREF forecolor;//前景色
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
		//	return;//用来防止一直刷屏 如果上次更新的状态和本次一样就不刷新按钮
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