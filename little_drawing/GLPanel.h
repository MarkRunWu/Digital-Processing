#ifndef GLPANEL_H
#define GLPANEL_H
#include "FL/Fl.H"
#include "FL/Fl_Gl_Window.H"
#include "GL/glut.h"
#include <vector>
#define M_PI 3.141592654
#include "Draw_Element.h"
#include "PointSet_Element.h"
#include "BSpline_Element.h"
using namespace std;

class GLPanel : public Fl_Gl_Window{
public:
	uchar R,G,B;
	enum state{ LITTLE_DRAW, LITTLE_DELETE };
	GLPanel(int x, int y,int w,int h ):
	 Fl_Gl_Window( x,y,w,h){
		 R = G = B;
	 }
	~GLPanel();
	void setState(int state);
protected:
	void draw();
	int handle(int Event);
private:
	int delete_state_handle(int Event);
	int draw_state_handle(int Event);
	inline void drawLine( float x , float y , float x1 , float y1 );
	inline void drawCircle( float x , float y , float r );
	inline void drawBspline();
	vector<Draw_Element*> records;
	PointSet_Element drawing_element;
	bool state_drawing;
	int _state;
	littleDrawing_Point last_pt;
};

#endif