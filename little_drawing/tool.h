#ifndef TOOL_H
#define TOOL_H
#include <GL/glut.h>
#include <cmath>
#define M_PI 3.141592654
/*
void drawLine( float x , float y , float x1 , float y1 ){
	glBegin( GL_LINES );
	glVertex2f( x , y );
	glVertex2f( x1 , y1 );
	glEnd();
}
  */
static void drawCircle( float x , float y , float r){
	float dt = M_PI / 180;
	glBegin( GL_TRIANGLE_FAN );
	glVertex2f( x , y );
	for( float t = 0 ; t < 2*M_PI ; t += dt ){
		glVertex2f( x + r*cos(t) , y + r*sin(t) );
		glVertex2f( x + r*cos(t + dt) , y + r*sin(t + dt) );
	}
	glEnd();
}
  
#endif