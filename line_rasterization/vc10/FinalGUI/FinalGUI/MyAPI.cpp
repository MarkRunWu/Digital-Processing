#include <stdlib.h>
#include <stdio.h> 
#include <GL/glut.h>
#include <fstream>
using namespace std;
//extern fstream log_file;
void MyLine(int Px0 , int Py0 , int Px1 , int Py1 , fstream &log_file , bool b_log = false);
void MyPoint(int Px0 , int Py0 );

void MyLine(int Px0 , int Py0 , int Px1 , int Py1 , fstream &log_file , bool b_log){
    int step = 1;
  int step2 = 1; // y axis: -1
  int step3 = 1;
  
  int sign = 1;
  int d;
  int delE;
  int delNE;
  int dy = Py1 - Py0;
  int dx = Px1 - Px0;
  int condition = Px1;
  bool reverse = false;
  int tx = Px0;
  int ty = Py0;

  if (dy < 0) { // case 6 7
    dy = -dy;
    step = -1;
  }
  if (dx < 0) {
    step2 = -1;
    step3 = -1;
    sign = -1;
  }

  if ( abs(dy) < abs(dx) ) { // when slop is smaller
    d = 2*step2*dy - dx;
    delE = 2*step2*dy;
    delNE = 2*(step2*dy - dx);
  }
  else { // when slop is larger
    step2 = step; // if dy < 0 , -1
    int tmp = ty;
    ty = tx;
    tx = tmp;
    condition = Py1;
    reverse = true;
    sign = -1;

    d = dy - 2*dx;
    delE = -2*dx;
    delNE = 2*( step3*dy - dx);
  }

  if ( reverse ) {
    MyPoint(ty,tx);
	if( b_log )
		log_file << ty << "," << tx ;
  }
  else {
    MyPoint(tx,ty);
    if( b_log )
		log_file << tx << "," << ty ;
  }

  while ( step2*tx < step2*condition ) {
	if( b_log )
		log_file <<  "=>" ;
    if ( sign*d > 0 ) { //NE
      d += delNE;
      ty+=step*step2*step3;
    }
    else { // E
      d += delE;
    }
    tx += step2;
    if ( reverse ) {
      MyPoint(ty,tx);
	  if( b_log )
		log_file <<  ty << "," << tx ;
      //print( ty + "," + tx );
    }
    else {
      MyPoint(tx,ty);
	 if( b_log )
		log_file <<  tx << "," << ty ;
      //print( tx + "," + ty );
    }
  }
  if( b_log )
		log_file << endl;
  log_file.close();
}

void MyPoint(int ix , int iy){
	glBegin( GL_QUADS );
	glVertex2i( ix , iy );
	glVertex2i( ix + 1 , iy );
	glVertex2i( ix + 1 , iy + 1);
	glVertex2i( ix , iy + 1);
	glEnd();
}