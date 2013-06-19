#include "TextureStroke.h"



TextureStroke::~TextureStroke(void)
{
}

void TextureStroke::paint(){
	 unsigned char image[50*50*4];
	//const char* const *p = stroke1.data();
	//const char* data = *p;
	for( int i = 0 ; i != 2500*3 ; i+=3 ){
		image[i] = 255;
		image[i+1] = 0;
		image[i+2] = 0;
	}
	//printf( "%d %d %d " , stroke1.w() , stroke1.count() , stroke1.d() );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D , _tex );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
	glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGB , 50 ,50 , 0 , GL_RGB , GL_UNSIGNED_BYTE , image );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D , _tex );
	for(size_t i = 0 ;  i != pts.size() ; i++ ){
		//glColor3f( colors[i].r , colors[i].g , colors[i].b );

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
