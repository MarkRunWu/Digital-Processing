#include <stdlib.h>
#include <stdio.h> 
#include <GL/glut.h>
#include <fstream>
using namespace std;
extern fstream log_file;
extern bool b_antialiasing;
void MyLine(int Px0 , int Py0 , int Px1 , int Py1 , bool b_log );
void MyPoint(int Px0 , int Py0 );

void MyLine(int Px0 , int Py0 , int Px1 , int Py1 , bool b_log){
	int step = 1;
	int step2 = 1; // y axis: -1
	int s = 1;
	int d;
	int delE;
	int delNE;
	int dy = Py1 - Py0;
	int dx = Px1 - Px0;
	int condition = Px1;
	float m = dy / (float)dx;
	bool reverse = false;
	int tx = Px0;
	int ty = Py0; 


	if ( abs(dy) > abs(dx) ){// when slop is larger
		int tmp = dy;
		dy = dx;
		dx = tmp;
		reverse = true;
		tmp = ty;
		ty = tx;
		tx = tmp;
		condition = Py1;
	}
	if( dy < 0 ){
		step = -1;
	}
	if( dx < 0 ){
		step2 = -1;
	}
	if( dy * dx < 0 )
		s = -1;

	d = 2*s*dy - dx;
	delE = 2*s*dy;
	delNE = 2*(s*dy - dx);

	if ( reverse ) {
		if( b_log )
			log_file <<  ty << "," << tx ;
		else
			MyPoint(ty, tx );
	}
	else {
		if( b_log )
			log_file <<  tx << "," << ty ;
		else
			MyPoint(tx, ty );
	}
	float f = 1;
	float y = ty;
	while ( step2*tx < step2*condition  ) {
		if( b_log )
			log_file <<  "=>" ;

		if ( step2*d > 0 ) { //NE
			d += delNE;
			ty += step;
		}
		else { // E
			d += delE;
		}
		tx += step2;
		f =  abs( y - (int)y );
		
		if ( reverse ) {
			if( b_log )
				log_file <<  ty << "," << tx ;
			else{
				glColor3ub( 255*f ,255*f , 255*f );
				MyPoint(ty, tx );
				if(b_antialiasing){
					glColor3ub( 255*(1 - f) , 255*(1 - f) , 255*(1 - f)  );
					MyPoint(ty + step, tx  );
				}
			}
		}else {
			if( b_log )
				log_file <<  tx << "," << ty ;
			else{
				glColor3ub( 255*f ,255*f , 255*f );
				MyPoint(tx, ty );
				if( b_antialiasing ){
					glColor3ub( 255*(1 - f) , 255*(1 - f) , 255*(1 - f)  );
					MyPoint(tx, ty + step );
				}
			}
		}
		y += m;
	}
}

void MyPoint(int ix , int iy){
	glBegin( GL_QUADS );
	glVertex2i( ix , iy );
	glVertex2i( ix + 1 , iy );
	glVertex2i( ix + 1 , iy + 1);
	glVertex2i( ix , iy + 1);
	glEnd();
}