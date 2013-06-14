#include "PointSet_Element.h"

void PointSet_Element::paint(){
	glPointSize( _element_size );
	glBegin( GL_POINTS );
	for( vector<littleDrawing_Point>::iterator p = pts.begin() ; p != pts.end() ; ++p ){
		glVertex2f( p->x , p->y );
	}
	glEnd();
	glPointSize(1);
}