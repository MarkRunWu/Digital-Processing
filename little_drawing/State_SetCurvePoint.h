#pragma once
#include "state.h"
#include "BSpline_Element.h"
class State_SetCurvePoint :
	public state
{
public:
	State_SetCurvePoint(void);
	~State_SetCurvePoint(void);
	int mouse_push(int x, int y);
	int mouse_up(int x, int y);
	//Draw_Element* getDrawElement() {return bspline->clone(); }
private:
	BSpline_Element* bspline;
};

