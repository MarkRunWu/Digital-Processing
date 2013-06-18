#include "FL/Fl_Gl_Window.H"
#include <vector>
using namespace std;

#ifndef PRESENTATION_MODEL_H
#define PRESENTATION_MODEL_H

class Presentation_model{
public:
	Presentation_model(){
		R = G = B = 0;
	    Radius_begin = Radius_middle = Radius_end = 1;
	}
	void subcribe(Fl_Gl_Window* fl);
	void unSubscribe(Fl_Gl_Window* fl);
	uchar R,G,B;
	uchar begin_R,begin_G,begin_B;
	uchar middle_R,middle_G,middle_B;
	uchar end_R,end_G,end_B;
	float Radius_begin;
	float Radius_middle;
	float Radius_end;
	void notify(){
		for( vector<Fl_Gl_Window*>::iterator t = observers.begin() ; t != observers.end() ; ++t )
			(*t)->redraw();
	}
private:
	vector<Fl_Gl_Window*> observers;

};

#endif