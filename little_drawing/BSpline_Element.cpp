#include "BSpline_Element.h"

BSpline_Element::BSpline_Element(){
}
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
	
	float dwidth = 1/max(1 , count_seg);
	littleDrawing_Point tmp_pt;
	OMT::Point p,p0,p1;

	for( int i = 0 ; i != count_seg ; i++ ){
		p0 = cubic_spline.get_position( i , 0 );
		p1 = cubic_spline.get_position( i , 1 );
		float distance = sqrt( (p0[0] - p1[0])*(p0[0] - p1[0]) + (p0[1] - p1[1])*(p0[1] - p1[1]) );
		float dt = 1.0/distance;
		
		for( float t = 0 ; t < 1 + dt ; t += dt  ){
			p = cubic_spline.get_position( i , t );
			tmp_pt.x = p[0];
			tmp_pt.y = p[1];
			pts.push_back( tmp_pt );
		}
	}

	//setStroke();

	//for(int i = 0 ; i != 5 ; i ++)
//		cout << R[i] << endl;
	
	updateBox();
}

void BSpline_Element::setStroke(float begin, float middle, float end){
	R[0] = begin;
	R[2] = middle;
	R[4] = end;
	R[1] = (R[0]*0.25 + R[2]* 0.25)*2;
	R[3] = (R[2]*0.25 + R[4]* 0.25)*2;

	int num = pts.size() - 3* (pts.size() / 4);
	int sum = 0;
	int j = 0;
	float dx;
	float r;
	for( int i = 0 ; i != pts.size() ; i++ ){
		if( sum == 0 || i >= sum ){
			dx =  (R[j+1] - R[j])/num;
			sum += num;
			num = pts.size() / 4;
			r =  R[j];
			j++;
			//cout << r << endl;
		}
		stroke_widths.push_back( r );
		r += dx;		
	}
}
void BSpline_Element::paint(){
	if( pts.size() ){
		glColor3ub( r,g,b );
		/*
		glLineWidth( 10 );
		glBegin( GL_LINES );
		for(size_t i = 0 ;  i != pts.size() - 1 ; i++ ){
			//drawCircle( pts[i].x , pts[i].y , 3 );
			glVertex2f( pts[i].x , pts[i].y );
			glVertex2f( pts[i+1].x , pts[i+1].y );
		}
		glEnd();
		  */
		for(size_t i = 0 ;  i != pts.size() ; i++ ){
			if( stroke_widths.size() )
				drawCircle( pts[i].x , pts[i].y , stroke_widths[i] );
			else drawCircle( pts[i].x , pts[i].y , 1 );
		} 
		glLineWidth( 1 );
	}
}