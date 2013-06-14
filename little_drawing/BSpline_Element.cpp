#include "BSpline_Element.h"

BSpline_Element::BSpline_Element(vector<littleDrawing_Point> pt){
	vector< OMT::Vector3d > v;
	OMT::Vector3d tmp;
	int count = pt.size() / 3;
	
	for(size_t i = 0 ; i != pt.size() ; i++ ){
		if( i % 10 ){
			tmp[0] = pt[i].x;
		    tmp[1] = pt[i].y;
			tmp[2] = 0;
			v.push_back( tmp ); 
		}
	}
	cubic_spline.setup_CSpline( v );
	int count_seg = cubic_spline.n_segs();
	littleDrawing_Point tmp_pt;
	OMT::Point p;
	for( int i = 0 ; i != count_seg ; i++ ){
		for( float t = 0 ; t < 1 + 0.1 ; t += 0.1 ){
			p = cubic_spline.get_position( i , t );
			tmp_pt.x = p[0];
			tmp_pt.y = p[1];
			pts.push_back( tmp_pt );
		}
	}
	updateBox();
}
void BSpline_Element::paint(){
	if( pts.size() ){
		glColor3ub( r,g,b );
		glBegin( GL_LINES );
		for(size_t i = 0 ;  i != pts.size() - 1 ; i++ ){
			glVertex2f( pts[i].x , pts[i].y );
			glVertex2f( pts[i+1].x , pts[i+1].y );
		}
		glEnd();
	}
}