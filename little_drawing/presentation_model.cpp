#include "presentation_model.h"

void Presentation_model::subcribe( Fl_Gl_Window* fl){
	for( vector<Fl_Gl_Window*>::iterator t = observers.begin() ; t != observers.end() ; ++t )
		if( (*t) == fl )return;
	observers.push_back( fl );
}

void Presentation_model::unSubscribe( Fl_Gl_Window* fl){
	for( vector<Fl_Gl_Window*>::iterator t = observers.begin() ; t != observers.end() ; ++t ){
		if( (*t) == fl ){
			observers.erase( t );
			return;
		}
	}
}