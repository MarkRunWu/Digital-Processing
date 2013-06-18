#include "PointSet_Element.h"

void PointSet_Element::paint(){
	glPointSize( _element_size );
	if( pts.size() > 0 ){
	glBegin( GL_LINES );
		for( size_t i = 0 ; i != pts.size() - 1 ; i++  ){
			glVertex2f( pts[i].x , pts[i].y );
			glVertex2f( pts[i+1].x , pts[i+1].y );
		}
		glEnd();
	}
	
	glPointSize(1);
}