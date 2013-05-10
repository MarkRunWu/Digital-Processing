/***
Remainder Requiremets:
  document - readme.pdf
  input fields x y
  log.txt - contains begin_pt and end_pt : a sequence points from begin_pt to end_pt
*/
int begin_pt[] = new int[2];
int end_pt[] = new int[2];
int count = 0;
float ss = 1;
boolean b_log = false;
boolean b_antialiasing = false;
String log;
PrintWriter file;
ArrayList<PVector> triangle = new ArrayList<PVector>();
PImage Image_NoAccept;
class HalfSpaceMap{
  public void add(float[] es ){
    float copy_es [] = new float [es.length];
    boolean t_diff = (es[0] > 0);
    boolean f_diff = (es[0] < 0);
    
    for(int i = 0  ; i != es.length ; i++ ){
      t_diff &= ( es[i] > 0 );
      f_diff &= ( es[i] < 0 );
      copy_es[i] = es[i];
    }
    b_maps.add( t_diff | f_diff );
    maps.add( copy_es );
  }
  public void add(boolean passed ){
    b_maps.add( passed );
  }
  public void clear(){
    maps.clear();
    b_maps.clear();
  }
  public int size(){
    return maps.size();
  }
  public boolean is_passed(int x, int y){
    if( x > (xmax - xmin) || x < 0 || y > (ymax - ymin) || y < 0 )
      return false;
    return is_passed( y * (int)(xmax - xmin + 1) + x );
  }
  public boolean is_passed(int index){
    if( b_maps.size() > 0 && index >= 0 && index < b_maps.size() ){
        return b_maps.get(index);
    }
    return false;
  }
  public float[] get(int index){
    if( maps.size() <= 0 || index < 0 || index > maps.size() )return null;
    return maps.get( index );
  }
  public float[] get(int x,int y){
    if( x > (xmax - xmin) || x < 0 || y > (ymax - ymin) || y < 0 )
      return null;
    return get( y * (int)(xmax - xmin + 1) + x );
  }
  
  public int getX(int index){
    return (int)xmin + index % (int)(xmax - xmin + 1);
  }
  public int getY(int index){
    return (int)ymin + index / (int)(xmax - xmin + 1);
  }
  
  public float xmin;
  public float ymin;
  public float xmax;
  public float ymax;
  public ArrayList<float[]> maps = new ArrayList<float[]>();
  public ArrayList<Boolean> b_maps = new ArrayList<Boolean>();
}

HalfSpaceMap maps = new HalfSpaceMap();

boolean b_LineNormal = true;
void doLog(){
  b_log = true;
  log = "";
  file = createWriter("log.txt");
  draw_line(  begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
  file.print( log );
  file.flush();
  file.close();
  b_log = false;
}

boolean[] getAcceptMarker(int size){
  int i = ( mouseX - 20*(int)maps.xmin - 20)/20;
  int j = ( mouseY - 20*(int)maps.ymin - 20)/20;
   
  boolean results[] = new boolean[size];
  for( int k = 0 ; k != size ; k++ )
    results[k] = true;
      
  float[] p_detail = maps.get( i , j );
  if( p_detail != null && !maps.is_passed( i , j ) ){
      for(int k = 0 ; k != p_detail.length ; k++ ){
        if(p_detail[k] >= 0)
          results[k] = false;
      }
  }
  return results;
}
void showTextMenu(){
  /* will fix later*/
  stroke(255);
  fill(0);
  String anti = "Anti-aliasing: " + (b_antialiasing ? "(Enable)" : "(Disable)");
  text( anti , 10 , 10 );
  
  /*
  String t = "points num: " + triangle.size(); 
  text( t  , mouseX + 10 , mouseY + 50);
  */
  int i,j,index;
  i = ( mouseX - 20*(int)maps.xmin - 20)/20;
  j = ( mouseY - 20*(int)maps.ymin - 20)/20;
  
  float[] p_detail = maps.get( i , j );
  
  textAlign(LEFT,BOTTOM);
  if( p_detail != null ){
    String pixel_content = "i: " + i + ",j: " + j + "\n";// e1: " + p_detail.x + "\n,e2: " + p_detail.y +  "\n,e3: " + p_detail.z;
    for(int p = 0 ; p != p_detail.length ; p++ ){
      pixel_content += "e" + p + ": " + p_detail[p] + "\n"; 
    }
    
    fill(255);
    rect( mouseX + 5 , mouseY + 10 , 200 , 200);
    fill(0); 
    text( pixel_content  , mouseX + 20 , mouseY + 150 );
    
    stroke( 255 , 0 , 0 );
    writePixels( i + (int)maps.xmin + 1 , j  + (int)maps.ymin + 1 , 20 );
  }else{
    stroke(255,0,255);
    fill(255,255,0);
    writePixels( mouseX/20 , mouseY/20 , 20 );
  }
  fill( 255 , 0 , 255);
  //rect( mouseX + 10 , mouseY + 30 , t.length() * 16 , 20 ); 
  stroke(0);
}

void setup() {
   Image_NoAccept = loadImage( "NoAcceptMarker.png" );
  size(520, 520);
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
void writeLine( int x, int y ,int x1 , int y1 , String tag , color fc , boolean accept){
   x = 20*x + 10;   y = 20*y + 10; 
  x1 = 20*x1 + 10; y1 = 20*y1 + 10;
  strokeWeight( 2 );
  stroke( fc );
  line( x , y , x1 , y1 ); 
  
  float center_x = 0.5*x + 0.5*x1;
  float center_y = 0.5*y + 0.5*y1;
  
  fill( 255 , 255 , 255 , 127 );
  ellipse( center_x , center_y , 30 , 30 );
  fill( 0 );
  textAlign( CENTER , CENTER );
  text( tag , center_x , center_y );
  strokeWeight( 1 ); 
  
  if( !accept ){
    image( Image_NoAccept , center_x - 15 , center_y - 15 , 30 , 30 ); 
  }
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


boolean windingtNumberTestI( ArrayList<PVector> pts , float x , float y){
  float angle = 0;
  PVector point = new PVector( x , y ); 
  for( int i = 0 ; i != pts.size() - 1; i++){
    PVector p0 = new PVector( pts.get(i).x , pts.get(i).y );
    PVector p1 = new PVector( pts.get(i+1).x , pts.get(i+1).y );
    
    p0.sub( point );
    p1.sub( point );
    
    p0.normalize();
    p1.normalize();
    
    angle += acos( p0.dot( p1 ) );
  }
  
  if( angle - 2*PI > -1e-10 && angle - 2*PI < 1e+10 ){
    return true;
  }
  return false;
}

boolean windingtNumberTestII( ArrayList<PVector> pts ){  
  for( int i = 0 ; i < pts.size() - 2 ; i++ ){
    //select two line
    PVector p0 = pts.get(i);
    PVector p1 = pts.get(i + 1);
    PVector p2 = pts.get(i + 2);
    
    //continue from next line
    for( int j = i  + 1 ; j < pts.size() ; j++ ){
      
    }
  }
  return false;
}

float lineEq( PVector p0 , PVector p1 , float x ,float y , PVector n){
  PVector p = new PVector(p0.x , p0.y);
  p.sub(p1);
  p.normalize();
  n.x = p.y; n.y = -p.x;
  float c = -( n.x * p0.x + n.y * p0.y ); 
  return n.x * x + n.y * y + c; 
}
void half_space_draw( ArrayList<PVector> pts ){
  if( pts.size() < 3) return;
  stroke( 255 , 0 , 255);
  fill( 255 );
  writePixels( (int)pts.get(0).x , (int)pts.get(0).y , 20);
  writePixels( (int)pts.get(1).x , (int)pts.get(1).y , 20);
  writePixels( (int)pts.get(2).x , (int)pts.get(2).y , 20);
  
  if( b_LineNormal ){
    for( int i = 0 ; i <= 2 ; i++){
      int i0,i1;
      i0 = i;
      i1 = (i + 1)%3;
      
      PVector p = new PVector(pts.get(i0).x , pts.get(i0).y);
      p.sub(pts.get(i1));
      p.normalize();
      PVector n = new PVector(p.y , -p.x);
      
      PVector line_center = new PVector( (pts.get(i0).x + pts.get(i1).x)*0.5 , (pts.get(i0).y + pts.get(i1).y)*0.5 );
      //fill( 255 * ((i+1)&0x01) , 255 * ((i+1)&(0x02)) , 255 * ((i+1)&(0x03)) );
      strokeWeight(4);
      stroke(255,0,255);
      ellipse(10 + 20*line_center.x , 10 + 20*line_center.y , 20 , 20 );
      line( 10 + 20*line_center.x , 10 + 20*line_center.y , 10 + 20*(line_center.x + 2*n.x) , 10 + 20*(line_center.y + 2*n.y) );
      line( 10 + 20*pts.get(i0).x , 10 + 20*pts.get(i0).y , 10 + 20*pts.get(i1).x , 10 + 20*pts.get(i1).y );
      strokeWeight(1);
      stroke( 0 );
      fill( 255 );
    }
  }
}
void fill_polygon(  ArrayList<PVector> pts ){
  if( pts.size() < 3 )return;
  float xmin = 1e+30;
  float ymin = 1e+30;
  
  float xmax = 1e-30;
  float ymax = 1e-30;
  
  for( int i = 0 ; i != pts.size() ; i++ ){
    xmin = min( xmin , pts.get(i).x );
    ymin = min( ymin , pts.get(i).y );
    
    xmax = max( xmax , pts.get(i).x );
    ymax = max( ymax , pts.get(i).y );
  }
  maps.xmin = xmin; maps.ymin = ymin;
  maps.xmax = xmax; maps.ymax = ymax;
  
  /* half space tests
  float xDim =  xmax - xmin + 1;
  float en[] = new float[pts.size()];
  PVector n[] = new PVector[pts.size()];
  for( int i = 0 ; i != pts.size() ; i++ ){
    n[i] = new PVector();
    en[i] = lineEq( pts.get(i) , pts.get( (i+1)%pts.size() ) , xmin , ymin , n[i]);
  }
   for( int y = (int)ymin ; y <= (int)ymax ; y++ ){
    for( int x = (int)xmin ; x <= (int)xmax ; x++ ){
      maps.add( en );
      for( int e_num = 0 ; e_num != pts.size() ; e_num++ )
        en[ e_num ] += n[e_num].x;
    }
    for( int e_num = 0 ; e_num != pts.size() ; e_num++ ) 
     en[ e_num ] += -xDim*n[e_num].x + n[e_num].y; 
   }
   
   /* winding num tests*/
   for( int y = (int)ymin ; y <= (int)ymax ; y++ ){
    for( int x = (int)xmin ; x <= (int)xmax ; x++ ){
      maps.add( windingtNumberTestI( pts , x , y ) );
    }
   }
}

//Assume pts are already in order CCW / CW
void fill_triangle( ArrayList<PVector> pts){
 
  if( pts.size() < 3 )return;
  
  //calculate bouding box
  float xmin = min( min( pts.get(0).x , pts.get(1).x ) , pts.get(2).x );
  float ymin = min( min( pts.get(0).y , pts.get(1).y ) , pts.get(2).y );
  
  float xmax = max( max( pts.get(0).x , pts.get(1).x ) , pts.get(2).x );
  float ymax = max( max( pts.get(0).y , pts.get(1).y ) , pts.get(2).y );
  PVector n1,n2,n3;
  n1 = new PVector();
  n2 = new PVector();
  n3 = new PVector();
  stroke( 255 , 0 , 0 );
  float e1 = lineEq( pts.get(0) , pts.get(1) , xmin , ymin , n1);
  stroke( 0 , 255 , 0 );
  float e2 = lineEq( pts.get(1) , pts.get(2) , xmin , ymin , n2);
  stroke( 0 , 0 , 255 );
  float e3 = lineEq( pts.get(2) , pts.get(0) , xmin , ymin , n3);
  
  //dump normal
  println( "o: " + n1.x + "," + n1.y + "e1: " + e1 );
  println( "o1: " + n2.x + "," + n2.y + "e2: " + e2  );
  println( "o2: " + n3.x + "," + n3.y + "e3: " + e3  );
  
  float a1 = n1.x;
  float a2 = n2.x;
  float a3 = n3.x;
  
  float b1 = n1.y;
  float b2 = n2.y;
  float b3 = n3.y;
  
 
  fill( 255,0,0 );
  
  maps.xmin = xmin;
  maps.ymin = ymin;
  maps.xmax = xmax;
  maps.ymax = ymax;
  /*
  for( int y = (int)ymin ; y <= (int)ymax ; y++ ){
    float ee1 = e1;
    float ee2 = e2;
    float ee3 = e3;
    for( int x = (int)xmin ; x <= (int)xmax ; x++ ){
      maps.add( new PVector( ee1 , ee2 , ee3 ) );
      ee1 += a1;
      ee2 += a2;
      ee3 += a3; 
    }
    e1 +=  b1;
    e2 +=  b2;
    e3 +=  b3;
  }
  */
  /*** fixed error*/
  float e[] = new float[3];
   float xDim =  xmax - xmin + 1; // strike xDim step
 for( int y = (int)ymin ; y <= (int)ymax ; y++ ){
    for( int x = (int)xmin ; x <= (int)xmax ; x++ ){
      e[0] = e1; e[1] = e2; e[2] = e3;
      maps.add( e );
      e1 += a1;
      e2 += a2;
      e3 += a3;
    }
    e1 += -xDim*a1 + b1;
    e2 += -xDim*a2 + b2;
    e3 += -xDim*a3 + b3;
  } 
  /**/
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
  /*
  if (reverse)println(", result: " + (tx == Py1 && ty == Px1) );
  else println(", result: " + (tx == Px1 && ty == Py1) );
  fill(0);
  writePixels( Px0, Py0, 20);
  fill(0);
  writePixels( Px1, Py1, 20);
  */
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
   translate(20,20);
   ScreenPixels( 25 , 25 , 20 );
   /*
   if( count > 0 ){
   writePixels( begin_pt[0] , begin_pt[1] , 20 );
   if( count > 1){
   writePixels( end_pt[0] , end_pt[1] , 20 );
   draw_line( begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
   stroke(255,0,0);
  line( 10 + 20*begin_pt[0] , 10 + 20*begin_pt[1] , 10 + 20*end_pt[0] , 10 + 20*end_pt[1] );
  }
   }
  */
  
  float e_map[];
  for(int i = 0 ; i != maps.size() ; i++){
//    float e1,e2,e3;
    if( maps.is_passed(i) )
        fill(255, 255 , 0);
    else fill(127);//fill( e1 < 0 ? 255 : 0 , e2 < 0 ? 255 : 0 , e3 < 0 ? 255 : 0 );
    writePixels( maps.getX(i) , maps.getY(i) , 20);
  }
  //half_space_draw( triangle );
  if( triangle.size() > 1){
    int size = triangle.size();
    boolean b_accept[] = getAcceptMarker(size);
    for(int i = 0 ; i != size ; i++ ){
       writeLine( (int)triangle.get(i).x , (int)triangle.get(i).y ,  (int)triangle.get( (i+1)%size ).x  , (int)triangle.get( (i+1)%size ).y , "e" + i , color( 255 * (i&0x04) , 255 * (i&0x02) , 255 * (i&0x01) ) , b_accept[i] );
    }
  }
  for(int i = 0 ; i != triangle.size() ; i++ ){
     stroke(0);
     fill(255);
     writePixels(  (int)triangle.get(i).x , (int)triangle.get(i).y , 20 );
  }

  stroke(0);
   
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
    print( key );
  }
   redraw();
}
void mousePressed() {
  if( mouseButton == LEFT ){
    /*
    if( triangle.size() >= 3 ){
      triangle.clear();
      maps.clear();
    }
    triangle.add( new PVector( (mouseX - 20)/20 , (mouseY - 20)/20 ) );
    if( triangle.size() >= 3 ) fill_triangle( triangle );
    */
    maps.clear();
    triangle.add( new PVector( (mouseX - 20)/20 , (mouseY - 20)/20 ) );
    fill_polygon(triangle);
  }else{
    maps.clear();
    triangle.clear();
  }
  /*
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
  */
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

