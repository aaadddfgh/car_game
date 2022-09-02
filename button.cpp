#include "button.h"

#include<tchar.h>


button::button(int top, int left, int right, int bottom, const wchar_t* text){
	onColor = BLACK;//鼠标移动在按钮控件的颜色设置
	outColor = WHITE;//鼠标移开的颜色设置
	downColor = RED;//鼠标按下后的颜色；
	forecolor = BLUE;//前景色
	state.mousedown = false;//按钮状态设置
	state.mouseon = false;//按钮状态设置
	visible = true;		//按钮位置设置
	re.top = top;
	re.left = left;
	re.right = right;
	re.bottom = bottom;
	this->text = new WCHAR[10];
	wsprintfW(this->text,L"%s", text);
}
