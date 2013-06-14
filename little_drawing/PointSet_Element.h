#include "Draw_Element.h"
#ifndef POINTSET_ELEMENT_H
#define POINTSET_ELEMENT_H

class PointSet_Element : public Draw_Element{
public:
	PointSet_Element(){}
	~PointSet_Element(){}
	void release(){ pts.clear(); }
	vector<littleDrawing_Point> getPts(){ return pts; }
	void paint();
};
#endif