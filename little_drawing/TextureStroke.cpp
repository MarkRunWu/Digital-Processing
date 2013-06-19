#include "TextureStroke.h"



TextureStroke::~TextureStroke(void)
{
}

void TextureStroke::paint(){
	glEnable( GL_TEXTURE_2D );
	
	glBindTexture( GL_TEXTURE_2D , _tex );
	for(size_t i = 0 ;  i != pts.size() ; i++ ){
		glColor3f( colors[i].r , colors[i].g , colors[i].b );

		/*
		if( stroke_widths.size() )
		drawCircle( pts[i].x , pts[i].y , stroke_widths[i] );
		else drawCircle( pts[i].x , pts[i].y , 1 );
		*/
	
		glBegin( GL_QUADS );
		glTexCoord2f( 0,0);
		glVertex2f( pts[i].x - stroke_widths[i]/2 , pts[i].y - stroke_widths[i]/2);
		glTexCoord2f( 1,0);
		glVertex2f( pts[i].x + stroke_widths[i]/2 , pts[i].y - stroke_widths[i]/2);
		glTexCoord2f( 1,1);
		glVertex2f( pts[i].x + stroke_widths[i]/2 , pts[i].y + stroke_widths[i]/2);
		glTexCoord2f( 0,1);
		glVertex2f( pts[i].x - stroke_widths[i]/2 , pts[i].y + stroke_widths[i]/2);
		glEnd();
	} 
	glDisable( GL_TEXTURE_2D );
}
