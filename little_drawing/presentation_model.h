#include "FL/Fl_Gl_Window.H"
#include <vector>
using namespace std;

#ifndef PRESENTATION_MODEL_H
#define PRESENTATION_MODEL_H

class Presentation_model{
public:
	void subcribe(Fl_Gl_Window* fl);
	void unSubscribe(Fl_Gl_Window* fl);
	uchar R,G,B;
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