#pragma once
#include "Draw_Element.h"
class state
{
public:
	state(void);
	~state(void);
	// �p�G�^�� return 1, ��L return 0
	virtual int mouse_push(int x,int y){return 0;}
	virtual int mouse_drag(int x,int y){return 0;}
	virtual int mouse_up(int x,int y){return 0;}
	virtual Draw_Element* getDrawElement() = 0;
};

