#pragma once
#include "FL/Fl_Gl_Window.H"
#include "FL/glut.H"
class MainGL : Fl_Gl_Window
{
public:
	MainGL(int x,int y,int w,int h,const char* l=0):
	  Fl_Gl_Window( x, y, w, h){}
	~MainGL(void);
	void load( const char* file_name);
	void resize(int,int,int,int);
	int handle(int);
protected:
	void draw();
};

