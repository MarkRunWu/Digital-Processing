
#include "GLPanel.h"


GLPanel::~GLPanel(){

}

void GLPanel::setState(int state ){
	_state = state;
	switch( _state ){
	case LITTLE_DRAW:
		cursor( FL_CURSOR_ARROW );
		break;
	case LITTLE_DELETE:
		cursor( FL_CURSOR_CROSS );
		break;
	default:
		cursor( FL_CURSOR_DEFAULT );
	}
}

int GLPanel::delete_state_handle(int Event ){
	switch(Event){
	case FL_PUSH:
		last_pt.x = Fl::event_x();
		last_pt.y = Fl::event_y();
		return 1;
	case FL_RELEASE:
		drawing_element.release();
		redraw();
		return 1;
	case FL_DRAG:
		littleDrawing_Point box_min,box_max;
		littleDrawing_Point pt;
		pt.x = Fl::event_x();
		pt.y = Fl::event_y();
		drawing_element.add( pt );
		vector<Draw_Element*> new_ones;
		vector<Draw_Element*> recycle_ones;
		for( vector<Draw_Element*>::iterator p = records.begin(); p != records.end() ; ++p ){	
			(*p)->getBox( box_min , box_max );
			bool b_hit = false;
			if( pt.x > box_min.x && pt.y > box_min.y && pt.x < box_max.x && pt.y < box_max.y || 
				last_pt.x > box_min.x && last_pt.y > box_min.y && last_pt.x < box_max.x && last_pt.y < box_max.y ){
					vector<littleDrawing_Point> points = (*p)->getPts();
					//line of last_pt and pt
					float vx,vy,c,l,d;
					vx = -last_pt.y + pt.y;
					vy = last_pt.x - pt.x;
					l = sqrt( vx*vx + vy*vy );
					vx /= l; vy /= l;
					c = -( vx*last_pt.x + vy*last_pt.y );
					
					for( vector<littleDrawing_Point>::iterator lp = points.begin(); lp != points.end() ; ++lp ){
						d = vx*lp->x - vy*lp->y + c;
						if(  d > 1 && d < 1  ){
							b_hit = true;
							break;
						}
					}
			}
			if( !b_hit ) new_ones.push_back(*p);
			else recycle_ones.push_back(*p);

		}
		recycle_ones.clear();
		records = new_ones;
		last_pt = pt;
		redraw();
		return 1;

	}
	return Fl_Gl_Window::handle(Event); 
}

int GLPanel::draw_state_handle(int Event ){
	switch(Event){
	case FL_PUSH:
		return 1;
	case FL_DRAG:
		littleDrawing_Point p;
		p.x = Fl::event_x();
		p.y = Fl::event_y();
		drawing_element.add( p );
		state_drawing = true;

		redraw();
		return 1;
	case FL_RELEASE:
		Draw_Element* pElment = new BSpline_Element( drawing_element.getPts() );
		pElment->setColor( R,G,B );
		records.push_back( pElment );
		drawing_element.release();
		state_drawing = false;

		redraw();
		return 1;
	}
	return Fl_Gl_Window::handle(Event);
}
int GLPanel::handle( int Event ){
	if( _state == LITTLE_DRAW ){
		return draw_state_handle( Event );
	}else if(  _state == LITTLE_DELETE ){
		return delete_state_handle( Event );
	}
	return Fl_Gl_Window::handle(Event);
}
void GLPanel::draw(){
	if( !valid() ){
		glClearColor(1.0,1.0,1.0,1.0);
		glViewport( 0 , 0 , w() , h() );
		glOrtho( 0 , w() , h() , 0 , 0 , 1);
	}
	glClear( GL_COLOR_BUFFER_BIT );
	glColor3ub(0,0,0);
	for( vector<Draw_Element*>::iterator p = records.begin(); p != records.end() ; ++p )
		(*p)->paint();
	if( state_drawing )
		drawing_element.paint();
}

void GLPanel::drawLine( float x , float y , float x1 , float y1 ){
	glBegin( GL_LINES );
	glVertex2f( x , y );
	glVertex2f( x1 , y1 );
	glEnd();
}

void GLPanel::drawCircle( float x , float y , float r){
	float dt = M_PI / 180;
	glBegin( GL_LINES );
	for( float t = 0 ; t < 2*M_PI ; t += dt ){
		glVertex2f( x + r*cos(t) , y + r*sin(t) );
		glVertex2f( x + r*cos(t + dt) , y + r*sin(t + dt) );
	}
	glEnd();
}