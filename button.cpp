#include "button.h"

#include<tchar.h>


button::button(int top, int left, int right, int bottom, const wchar_t* text){
	onColor = BLACK;//����ƶ��ڰ�ť�ؼ�����ɫ����
	outColor = WHITE;//����ƿ�����ɫ����
	downColor = RED;//��갴�º����ɫ��
	forecolor = BLUE;//ǰ��ɫ
	state.mousedown = false;//��ť״̬����
	state.mouseon = false;//��ť״̬����
	visible = true;		//��ťλ������
	re.top = top;
	re.left = left;
	re.right = right;
	re.bottom = bottom;
	this->text = new WCHAR[10];
	wsprintfW(this->text,L"%s", text);
}
