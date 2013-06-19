#pragma once
#include "bspline_element.h"
class TextureStroke :
	public BSpline_Element
{
public:
	TextureStroke(GLuint _texture , vector<littleDrawing_Point> pt ):
	  BSpline_Element( pt )
	{ _tex = _texture; 
	 
	  }
	~TextureStroke(void);
	void paint();
private:
	GLuint _tex;
};

