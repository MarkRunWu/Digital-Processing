
#include "GLPanel.h"


GLPanel::~GLPanel(){
	glDeleteTextures( 1 , &this->_texBackground );
	glDeleteTextures( 3 , _texStroke );
}

void GLPanel::setState(int state ){
	_state = state;
	//_pState = new State_SetCurvePoint();

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
littleDrawing_Point normalize( littleDrawing_Point p  ){
	littleDrawing_Point pt;
	float l = sqrt( p.x*p.x + p.y*p.y );
	pt.x =  p.x/l;
	pt.y =  p.y/l;
	return pt;
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
					littleDrawing_Point l_pt,r_pt;
					for( vector<littleDrawing_Point>::iterator lp = points.begin(); lp != points.end() ; ++lp ){
						l_pt.x = lp->x -  pt.x;
						l_pt.y = lp->y -  pt.y;
						r_pt.x = lp->x -  last_pt.x;
						r_pt.y = lp->y -  last_pt.y;
						l_pt = normalize( l_pt );
						r_pt = normalize( r_pt );
						//cout << l_pt.x - r_pt.x << endl;
						//cout << l_pt.y - r_pt.y << endl;
						if( l_pt.x - r_pt.x < 1e-2 && l_pt.x - r_pt.x > -1e-2  &&
							l_pt.y - r_pt.y < 1e-2 && l_pt.y - r_pt.y > -1e-2 ){
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
		this->b_modified = true;
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
		BSpline_Element* pElment;
		if( _model->stroke_type == 0){
			pElment =  new BSpline_Element( drawing_element.getPts() );
		}else{
			pElment = new TextureStroke( _texStroke[ _model->stroke_type - 1 ] , drawing_element.getPts() );
		}
		pElment->setColor( _model->R,_model->G,_model->B );
		pElment->setBeginColor( _model->begin_R / (float)255 , _model->begin_G/ (float)255 , _model->begin_B/ (float)255 );
		pElment->setMiddleColor( _model->middle_R/ (float)255 , _model->middle_G/ (float)255 , _model->middle_B / (float)255);
		pElment->setEndColor( _model->end_R/ (float)255 , _model->end_G/ (float)255 , _model->end_B/ (float)255 );
		pElment->setStroke( _model->Radius_begin , _model->Radius_middle , _model->Radius_end  );
		records.push_back( pElment );
		//cout << records.size() << endl;
		b_modified = true;
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
void GLPanel::save2background(){
	glReadPixels( 0 , 0 , w() , h() , GL_RGB , GL_UNSIGNED_BYTE , background );
    glBindTexture( GL_TEXTURE_2D , _texBackground );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
	glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGB , w() , h() , 0 , GL_RGB , GL_UNSIGNED_BYTE , this->background );
	
}
void GLPanel::draw(){
	if( !valid() ){
		if( background != NULL )delete []background;
		background = new unsigned char[w()*h()*4];
		glClearColor(1.0,1.0,1.0,1.0);
		glViewport( 0 , 0 , w() , h() );
		glOrtho( 0 , w() , h() , 0 , 0 , 1);
		glEnable( GL_POINT_SMOOTH );
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
	}
	if( !context_valid() ){
		glGenTextures( 1 , &this->_texBackground );
		glGenTextures( 3 , _texStroke );
	}else{
		if( !b_load_texture){
			loadStrokeTexture();
			b_load_texture = true;
		}
	}
	glClear( GL_COLOR_BUFFER_BIT );

	if( b_modified || b_disable_background ){
		for( vector<Draw_Element*>::iterator p = records.begin(); p != records.end() ; ++p )
			(*p)->paint();
		b_modified = false;
		save2background();
	}else{
		glBindTexture( GL_TEXTURE_2D , _texBackground );
		glEnable( GL_TEXTURE_2D );
		glColor3ub(255,255,255);
		glBegin( GL_QUADS );
		glTexCoord2i( 0 , 1 );
		glVertex2i( 0 , 0 );
		glTexCoord2i( 1 , 1 );
		glVertex2i( w() , 0 );
		glTexCoord2i( 1 , 0 );
		glVertex2i( w() , h() );
		glTexCoord2i( 0 , 0 );
		glVertex2i( 0 , h() );
		glEnd();
		glDisable( GL_TEXTURE_2D );
	}
	glColor3ub( 0 ,0 ,0 );
	//if( state_drawing )
	drawing_element.paint();
}

void GLPanel::loadStrokeTexture(){
	//const char* tex_source[] = { "stroke1.png" , "stroke2.png" , "stroke3.png" };
	
	glEnable( GL_TEXTURE_2D );
	Fl_PNG_Image stroke1( "stroke1.png" );
	Fl_PNG_Image stroke2( "stroke2.png" );
	Fl_PNG_Image stroke3( "stroke3.png" );

	unsigned char image[50*50*4];

	const char* data;
	//printf( "%d %d %d " , stroke1.w() , stroke1.count() , stroke1.d() );
	
	data = *stroke1.data();
	for( int i = 0 ; i != 50 ; i++ ){
		for( int j = 0 ; j != 200 ; j++ , ++data){
			image[i*200+j] = *data;
		}
	}
	glBindTexture( GL_TEXTURE_2D , _texStroke[0] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
	glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , stroke1.w() , stroke1.h() , 0 , GL_RGBA , GL_UNSIGNED_BYTE , image );
	
	
	data = *stroke2.data();
	for( int i = 0 ; i != 50 ; i++ ){
		for( int j = 0 ; j != 200 ; j++ , ++data){
			image[i*200+j] = *data;
		}
	}
	glBindTexture( GL_TEXTURE_2D , _texStroke[1] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
	glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , stroke2.w() , stroke2.h() , 0 , GL_RGBA , GL_UNSIGNED_BYTE , image );
	

	data = *stroke3.data();
	for( int i = 0 ; i != 50 ; i++ ){
		for( int j = 0 ; j != 200 ; j++ , ++data){
			image[i*200+j] = *data;
		}
	}
	glBindTexture( GL_TEXTURE_2D , _texStroke[2] );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
	glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , stroke3.w() , stroke3.h() , 0 , GL_RGBA , GL_UNSIGNED_BYTE , image );
	glDisable( GL_TEXTURE_2D );
	
}
