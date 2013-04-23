/***
Remainder Requiremets:
  document - readme.pdf
  input fields x y
  log.txt - contains begin_pt and end_pt : a sequence points from begin_pt to end_pt
*/
int region = 0;
boolean enter_num = false;
int enter_count = 0;
int num;
int begin_pt[] = new int[2];
int end_pt[] = new int[2];
int count = 0;
float ss = 1;
boolean b_log = false;
boolean b_antialiasing = false;

PrintWriter file;
void doLog(){
  b_log = true;
  file = createWriter("log.txt");
  file.println( "start: " + begin_pt[0] + "," + begin_pt[1] + ", end: " + end_pt[0] + "," + end_pt[1] );
  draw_line(  begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
  file.flush();
  file.close();
  b_log = false;
}

void showTextMenu(){
  /* will fix later*/
  fill(255);
  rect( 590 , 0 , 200 , 20 );
  if( enter_num && region == 0 )
     fill(255,255,0);
  else
    fill(255);
  rect( 590 , 20 , 100 , 30 );
  if( enter_num && region == 1 )
     fill(255,255,0);
  else
    fill(255);
  rect( 690 , 20 , 100 , 30 );
  if( enter_num && region == 2 )
     fill(255,255,0);
  else
    fill(255);
  rect( 590 , 50 , 100 , 30 );
  if( enter_num && region == 3 )
     fill(255,255,0);
  else
    fill(255);
  rect( 690 , 50 , 100 , 30 );
  
  fill(0);
  textAlign( LEFT , BOTTOM );
  stroke(255);
  String anti = "Anti-aliasing: " + (b_antialiasing ? "(Enable)" : "(Disable)");
  text( anti , 600 ,  20 );

  text( "X0: " + begin_pt[0] , 600 , 50 ); text( "Y0: " + begin_pt[1] , 700 , 50 );
  text( "X1: " + end_pt[0], 600 , 80 ); text( "Y1: " + end_pt[1] , 700 , 80 );
  
  
  
  stroke(0);
}

void setup() {
  size(800, 600);
  if (frame!=null) frame.setResizable(true);
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
  //testCase07();
  //testCase16();
  //testCase34();
  //testCase25();
  
}
void writePixels( int x, int y, float r ) {
  ellipse(  x*r + 0.5*r, y*r + 0.5*r, r, r );
}

void ScreenPixelLabels(int cols, int rows, float r) {
  textAlign(CENTER , CENTER);
  for ( int i = 0 ; i != cols ; i++ )
    text( i,  2*r + i*r , 0.5*r );
  textAlign(CENTER , CENTER);
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
void draw_line(int Px0 , int Py0 , int Px1 , int Py1 ) {
  int step = 1;
  int step2 = 1; // y axis: -1
  int s = 1;
  int d;
  int delE;
  int delNE;
  int dy = Py1 - Py0;
  int dx = Px1 - Px0;
  int condition = Px1;
  float m;
  boolean reverse = false;
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
  
  fill(0);  
  if ( reverse ) {
    
    if( b_log )
      file.print( ty + "," + tx );
    else
      writePixels(ty, tx, 20); 
  }
  else {
    
    if( b_log )
      file.print( tx + "," + ty );
    else
      writePixels(tx, ty, 20);
  }
  float f = 0;
  m = step*abs(dy / (float)dx);
  float y = ty + m;
  int caseNE;
  while ( step2*tx < step2*condition  ) {
    if( b_log )
      file.print(  "=>" );
    if ( step2*d > 0 ) { //NE
      d += delNE;
      ty += step;
      caseNE = step;
    }
    else { // E
      d += delE;
      caseNE = -step;
    }
    tx += step2;
    
    if( b_antialiasing ){
      f =  abs( y - ty );
    }
   
    //if( f > 0.5 ) f = 1 - f;
    if ( reverse ) {
      fill(255*f);
      //fill(0);
      if( b_log )
        file.print( ty + "," + tx);  
      else
        writePixels(ty, tx, 20);
      if( b_antialiasing ){
        fill(255*(1-f));
        writePixels(ty - caseNE, tx, 20);
      }
      
    }else {
      fill(255*f);
      //fill(0);
      if( b_log )
        file.print( tx + "," + ty );
      else
        writePixels(tx, ty, 20);
      if( b_antialiasing ){
        fill(255*(1-f));
        writePixels(tx, ty - caseNE, 20);
      }
    }
    y += m;
  }
 
  stroke(255,0,0);
  line( 10 + 20*begin_pt[0] , 10 + 20*begin_pt[1] , 10 + 20*end_pt[0] , 10 + 20*end_pt[1] );
  stroke(0);
}


void draw() {
  background(127);
  pushMatrix();
  scale(ss);
   fill(0);
   
   ScreenPixelLabels(25,25,20);
   translate(25,25);
   ScreenPixels( 25 , 25 , 20 );
   
   if( count > 0 ){
   writePixels( begin_pt[0] , begin_pt[1] , 20 );
   if( count > 1){
   writePixels( end_pt[0] , end_pt[1] , 20 );
   draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
   stroke(255,0,0);
  line( 10 + 20*begin_pt[0] , 10 + 20*begin_pt[1] , 10 + 20*end_pt[0] , 10 + 20*end_pt[1] );
  stroke(0);
   }
   }
  popMatrix();
  //testCase07();
  //testCase16();
  showTextMenu();
}
void keyPressed(){
  if( key == 's')
    ss -= 0.1;
  if( key == 'S')
    ss += 0.1;
  if( ss < 0.1 ) ss = 0.1;
  if( ss > 1 ) ss = 1;
  
  if( key == 'w' ){
     print( "saved to log file\n" ); 
     doLog();
  }
  if( key == 'a')
    b_antialiasing ^= true;
  if( key >= '0' && key <= '9' ){
    if( !enter_num ){
       enter_num = true;
       num = 0;
    }
    
    num  = num * 10 + (key - '0');
    print( key );
    switch( region ){
      case 0:
        count = 0;
        begin_pt[0] = num;
        break;
      case 1:
        begin_pt[1] = num;
        count += 1;
        break;
      case 2:
        end_pt[0] = num;
        break;
      case 3:
        end_pt[1] = num;
        count += 1;
        break;
    }
  }
  if( key == ' '){
    enter_count = 0;
    
    num = 0;
    region = ( region + 1)%4; 
    if( region == 0 ) enter_num = false;
  }
   redraw();
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
/*
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
  */
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

