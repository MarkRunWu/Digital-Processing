#ifndef GLPANEL_H
#define GLPANEL_H
#include "FL/Fl.H"
#include "FL/Fl_Gl_Window.H"
#include "GL/glut.h"
#include <vector>
#include "Draw_Element.h"
#include "PointSet_Element.h"
#include "BSpline_Element.h"
#include "TextureStroke.h"
#include "presentation_model.h"
#include "state.h"
#include "State_SetCurvePoint.h"
#include "FL/Fl_PNG_Image.H"

using namespace std;

class GLPanel : public Fl_Gl_Window{
public:
	Presentation_model* _model;
	enum state{ LITTLE_DRAW, LITTLE_DELETE, POINT_SET };
	void addModel( Presentation_model* model ){
		_model = model;
		_model->subcribe( this );
		_model->R = _model->G = _model->B = 0;
	}
	GLPanel(int x, int y,int w,int h ):
	 Fl_Gl_Window( x,y,w,h){
		 background = NULL;
		 b_modified = false;
		 b_disable_background = false;
		 b_load_texture = false;
	 }
	~GLPanel();
	void toggle_background( bool signal ){ b_disable_background = signal;  }
	void setState(int state);
protected:
	void draw();
	int handle(int Event);
private:
	bool b_disable_background;
	void loadStrokeTexture();
	state* _pState;
	void save2background();
	GLuint _texBackground;
	unsigned char* background;
	int delete_state_handle(int Event);
	int draw_state_handle(int Event);
	inline void drawBspline();
	vector<Draw_Element*> records;
	PointSet_Element drawing_element;
	bool state_drawing;
	int _state;
	littleDrawing_Point last_pt;
	bool b_modified;
	GLuint _texStroke[3];
	bool b_load_texture;
};

#endif