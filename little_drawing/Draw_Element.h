#ifndef DRAW_ELEMENT_H
#define DRAW_ELEMENT_H
#include<vector>
#include"GL/glut.h"
using namespace std;
typedef struct{
	float x;
	float y;
}littleDrawing_Point;
class Draw_Element{
public:
	void setSize( int s ){ _element_size = s; }
	virtual void paint() = 0;
	void add( littleDrawing_Point p ){ pts.push_back( p ); }
	void updateBox(){
		littleDrawing_Point min_pt;
		littleDrawing_Point max_pt;
		min_pt.y = min_pt.x = 1e30;
		max_pt.y = max_pt.x = -1e30;
		for( vector<littleDrawing_Point>::iterator p = pts.begin(); p!= pts.end(); ++p){
			min_pt.x = min( min_pt.x , p->x );
			max_pt.x = max( max_pt.x , p->x );
			min_pt.y = min( min_pt.y , p->y );
			max_pt.y = max( max_pt.y , p->y );
		}
		_minBox = min_pt;
		_maxBox = max_pt;
	}
	vector<littleDrawing_Point> getPts(){ return pts; }
	void getBox( littleDrawing_Point &minBox , littleDrawing_Point &maxBox ){ minBox = _minBox; maxBox = _maxBox;}
	void setColor( uchar r , uchar g , uchar b ){ this->r = r ; this->g = g; this->b = b; }
protected:
	littleDrawing_Point _minBox;
	littleDrawing_Point _maxBox;
	vector<littleDrawing_Point> pts;
	int _element_size;
	uchar r;
	uchar g;
	uchar b;
};
#endif