#include "tool.h"
#include "Draw_Element.h"
#include "Cubic_Spline.h"
#ifndef BSPLINE_ELEMENT_H
#define BSPLINE_ELEMENT_H

class BSpline_Element : public Draw_Element{
public :
	BSpline_Element();
	BSpline_Element(vector<littleDrawing_Point> pt);
	~BSpline_Element(){};
	void setStroke(float begin = 4 , float middle = 20 , float end = 5 );
	void paint();
private:
	float R[5]; //stroke parameters
	float begin;
	float middle;
	float end;
	CSpline::Cubic_spline cubic_spline;
	vector<float> stroke_widths;
};
#endif