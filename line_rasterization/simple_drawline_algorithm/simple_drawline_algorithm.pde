int begin_pt[] = new int[2];
int end_pt[] = new int[2];
int count = 0;
void setup() {
  size(520, 520);
  textSize(16);
  background(127);
  fill(0);
  ScreenPixelLabels(25, 25, 20);

  translate(20, 20);
  ScreenPixels( 25, 25, 20 );
  /* 
   if( count > 0 ){
   writePixels( begin_pt[0] , begin_pt[1] , 20 );
   if( count > 1){
   writePixels( end_pt[0] , end_pt[1] , 20 );
   draw_line( begin_pt , end_pt );
   }
   }
   */
  /***draw test
   \    2 | 1   /
   3  \    |   / 0
   ______\_| /________
   4    / | \   7
   /    |    \
   /  5   |  6   \
   */
  testCase07();
  //testCase16();
  testCase34();
  //testCase25();
}
void writePixels( int x, int y, float r ) {
  ellipse(  x*r + 0.5*r, y*r + 0.5*r, r, r );
}

void ScreenPixelLabels(int cols, int rows, float r) {
  textAlign(LEFT);
  for ( int i = 0 ; i != cols ; i++ )
    text( i, r + i*r, r );
  textAlign(CENTER);
  for ( int i = 0 ; i != rows ; i++ )
    text( i, 0.5*r, 2*r + i*r );
}
void ScreenPixels(int cols, int rows, float r) {
  fill( 255);
  float hr = 0.5*r;
  for ( int i = 0 ; i != cols ; i++ )
    for ( int j = 0 ; j != rows ; j++ )
      ellipse( i*r + hr, j*r + hr, r, r );
}

/***
 \    2 | 1   /
 3  \    |   / 0
 ______\_| /________
 4    / | \   7
 /    |    \
 /  5   |  6   \
 */
void draw_line(int Px0 , int Py0 , int Px1 , int Py1) {
  //dx
  //dy
  //d
  //init d 
  //case 1
  //if d > 0 ne , +a+b
  //if d < 0 e , +b
  //..
  /*** y = mx + b 
   -> dx*y = dy*x + b 
   -> dx*y - dy*x - b = 0
   */
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
  float m = dy / (float)dx;
  boolean reverse = false;
  int tx = Px0;
  int ty = Py0; 
  
  if (dy < 0) { // case 6 7
    //dy = -dy;
    step = -1;
  }
  if (dx < 0) {
    sign = -1;
    step2 = -1;
  }
  
  if ( abs(dy) < abs(dx) ){ // when slop is smaller
    d = 2*step2*step*dy - step2*dx;
    delE = 2*step2*step*dy;
    delNE = 2*step2*(step*dy - dx);
  }else { // when slop is larger
    step2 = step; // if dy < 0 , -1
    int tmp = ty;
    ty = tx;
    tx = tmp;
    condition = Py1;
    reverse = true;
    //sign = -1;

    d = dy - 2*dx;
    delE = -2*dx;
    delNE = 2*( step3*dy - dx);
  }

  if ( reverse ) {
    writePixels(ty, tx, 20);
    print( ty + "," + tx );
  }
  else {
    writePixels(tx, ty, 20);
    print( tx + "," + ty );
  }
  float f;
  float y = ty;
  while ( tx < condition  ) {
    print( "=>" );
    if ( sign*d > 0 ) { //NE
      d += delNE;
      ty += step;
    }
    else { // E
      d += delE;
    }
    tx += step2;
    f =  y - (int)y;
    if ( reverse ) {
      fill( 255*f );
      writePixels(ty, tx, 20);
      fill( 255*(1-f) );
      writePixels(ty + 1, tx , 20);
      y += m;
      print( ty + "," + tx );
    }
    else {
      fill( 255*f );
      writePixels(tx, ty, 20);
      fill( 255*(1 - f) );
      writePixels(tx, ty + 1 , 20);
      y += m;
      print( tx + "," + ty );
    }
  }
  if (reverse)println(", result: " + (tx == Py1 && ty == Px1) );
  else println(", result: " + (tx == Px1 && ty == Py1) );
  fill(0);
  writePixels( Px0, Py0, 20);
  fill(0);
  writePixels( Px1, Py1, 20);
}


void draw() {
  /*
  background(127);
   fill(0);
   ScreenPixelLabels(25,25,20);
   
   translate(20,20);
   ScreenPixels( 25 , 25 , 20 );
  /* 
   if( count > 0 ){
   writePixels( begin_pt[0] , begin_pt[1] , 20 );
   if( count > 1){
   writePixels( end_pt[0] , end_pt[1] , 20 );
   draw_line( begin_pt , end_pt );
   }
   }
   */
  //testCase07();
  //testCase16();
}
void mousePressed() {
  if ( count >= 2 )count = 0;
  if ( count == 0 ) {
    begin_pt[0] = mouseX;
    begin_pt[1] = mouseY;
    begin_pt[0] /= 20;
    begin_pt[1] /= 20;
  }
  else {
    end_pt[0] = mouseX;
    end_pt[1] = mouseY;
    end_pt[0] /= 20;
    end_pt[1] /= 20;
  }
  count++;
}

void testCase07() {
  //case 0,7
  println( "case 07: ");
  fill( 255, 255, 0);
  begin_pt[0] = 0; 
  begin_pt[1] = 10;
  end_pt[0] = 10; 
  end_pt[1] = 5;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
  fill( 255, 255, 0);
  begin_pt[0] = 0; 
  begin_pt[1] = 10;
  end_pt[0] = 20; 
  end_pt[1] = 5;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );

  fill( 255, 0, 0);
  begin_pt[0] = 0; 
  begin_pt[1] = 0;
  end_pt[0] = 10; 
  end_pt[1] = 25;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
  fill( 255, 0, 0);
  begin_pt[0] = 0; 
  begin_pt[1] = 0;
  end_pt[0] = 10; 
  end_pt[1] = 5;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
}

void testCase16() {
  println( "case 16: ");
  //case 1,6
  fill( 255, 255, 0);
  begin_pt[0] = 11; 
  begin_pt[1] = 10;
  end_pt[0] = 12; 
  end_pt[1] = 0;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
  fill( 255, 255, 0);
  begin_pt[0] = 13; 
  begin_pt[1] = 20;
  end_pt[0] = 15; 
  end_pt[1] = 0;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );

  fill( 255, 0, 0);

  begin_pt[0] = 0; 
  begin_pt[1] = 0;
  end_pt[0] = 5; 
  end_pt[1] = 10;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );

  fill( 255, 0, 0);
  begin_pt[0] = 11; 
  begin_pt[1] = 0;
  end_pt[0] = 12; 
  end_pt[1] = 10;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
}

void testCase34() {
  println( "case 34: ");
  fill( 255, 255, 0 );
  begin_pt[0] = 10; 
  begin_pt[1] = 6;
  end_pt[0] = 5; 
  end_pt[1] = 5;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
  fill( 255, 0, 0 );
  begin_pt[0] = 10; 
  begin_pt[1] = 7;
  end_pt[0] = 5; 
  end_pt[1] = 10;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
}

void testCase25() {
  println( "case 25: ");
  fill( 255, 255, 0 );
  begin_pt[0] = 5; 
  begin_pt[1] = 10;
  end_pt[0] = 2; 
  end_pt[1] = 5;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
  fill( 255, 0, 0 );
  begin_pt[0] = 6; 
  begin_pt[1] = 2;
  end_pt[0] = 5; 
  end_pt[1] = 10;
  draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
  
}

