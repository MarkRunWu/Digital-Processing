#include "State_SetCurvePoint.h"


State_SetCurvePoint::State_SetCurvePoint(void)
{
	bspline = new BSpline_Element();
}


State_SetCurvePoint::~State_SetCurvePoint(void)
{
	delete bspline;
}

int State_SetCurvePoint::mouse_push(int x,int y){
	
	return 1;
}
int State_SetCurvePoint::mouse_up(int x,int y){
	littleDrawing_Point pt;
	pt.x = x;
	pt.y = y;
	bspline->add( pt );
	return 1;
}
