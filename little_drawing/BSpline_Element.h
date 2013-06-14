#include "Draw_Element.h"
#include "Cubic_Spline.h"
#ifndef BSPLINE_ELEMENT_H
#define BSPLINE_ELEMENT_H

class BSpline_Element : public Draw_Element{
public :
	BSpline_Element(vector<littleDrawing_Point> pt);
	~BSpline_Element(){};
	void paint();
private:
	CSpline::Cubic_spline cubic_spline;
};
#endif