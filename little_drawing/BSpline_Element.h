#include "tool.h"
#include "Draw_Element.h"
#include "Cubic_Spline.h"
#ifndef BSPLINE_ELEMENT_H
#define BSPLINE_ELEMENT_H

typedef struct {
	float r;
	float g;
	float b;
}RGB;
class BSpline_Element : public Draw_Element{
public :
	BSpline_Element();
	BSpline_Element(vector<littleDrawing_Point> pt);
	~BSpline_Element(){};
	void setStroke(float begin = 4 , float middle = 20 , float end = 5 );
	void setBeginColor( float r,float  g , float b){ C[0].r = r; C[0].g = g; C[0].b = b; }
	void setMiddleColor( float r,float  g , float b){ C[2].r = r; C[2].g = g; C[2].b = b; }
	void setEndColor( float r,float  g, float b){ C[4].r = r; C[4].g = g; C[4].b = b; }
	void paint();
private:
	float R[5]; //stroke parameters
	RGB C[5];
	float begin;
	float middle;
	float end;
	CSpline::Cubic_spline cubic_spline;
	vector<float> stroke_widths;
	vector<RGB> colors;
};
#endif