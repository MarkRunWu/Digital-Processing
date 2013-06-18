#pragma once
#include "Draw_Element.h"
class state
{
public:
	state(void);
	~state(void);
	// 如果回應 return 1, 其他 return 0
	virtual int mouse_push(int x,int y){return 0;}
	virtual int mouse_drag(int x,int y){return 0;}
	virtual int mouse_up(int x,int y){return 0;}
	virtual Draw_Element* getDrawElement() = 0;
};

