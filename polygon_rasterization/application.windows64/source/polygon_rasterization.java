import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class polygon_rasterization extends PApplet {

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
  public static final int HALFSPACE = 0;
  public static final int WINDING_I = 1;
  public static final int WINDING_II = 2;
  private int mode = HALFSPACE;
  
  public void setMode( int mode ){
    this.mode = mode; 
  }
  private void halfspaceTest(float[] e){
     boolean t_diff = (e[0] > 0);
     boolean f_diff = (e[0] < 0);
     for(int i = 0  ; i != e.length ; i++ ){
        t_diff &= ( e[i] > 0 );
        f_diff &= ( e[i] < 0 );
     }
     b_maps.add( t_diff | f_diff );
  }
  private void wildingTest( float[] angles ){
    float test_angle = 0;
    for(int i = 0 ; i != angles.length ; i++){
       test_angle += angles[i];
    }
    b_maps.add( test_angle - 2*PI > -1e-5f && test_angle - 2*PI < 1e-5f );
  }
  
  private void wildingTest2( float[] times ){
    //b_maps.add(   
  }
  public void add(float[] data ){
    float copy_data [] = new float [data.length];
    for(int i = 0  ; i != data.length ; i++ ){
      copy_data[i] = data[i];
    }
    maps.add( copy_data );
    if( mode == HALFSPACE ){
      halfspaceTest( copy_data);
    }else if( mode == WINDING_I ){
      wildingTest( copy_data );
    }else if( mode == WINDING_II ){
      halfspaceTest( copy_data );
    }
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
public void doLog(){
  b_log = true;
  log = "";
  file = createWriter("log.txt");
  draw_line(  begin_pt[0] , begin_pt[1] , end_pt[0] , end_pt[1] );
  file.print( log );
  file.flush();
  file.close();
  b_log = false;
}

public boolean[] getAcceptMarker(int size){
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
public void showTextMenu(){
  /* will fix later*/
  /*stroke(255);
  fill(0);
  String anti = "Anti-aliasing: " + (b_antialiasing ? "(Enable)" : "(Disable)");
  text( anti , 10 , 10 );
  */
  /*
  String t = "points num: " + triangle.size(); 
  text( t  , mouseX + 10 , mouseY + 50);
  */
  int i,j,index;
  i = ( mouseX - 20*(int)maps.xmin - 20)/20;
  j = ( mouseY - 20*(int)maps.ymin - 20)/20;
  
  float[] p_detail = maps.get( i , j );
  
  textAlign(LEFT,TOP);
  if( p_detail != null ){
    String pixel_content = "i: " + i + ",j: " + j + "\n";// e1: " + p_detail.x + "\n,e2: " + p_detail.y +  "\n,e3: " + p_detail.z;
    
    float sum = 0;
    for(int p = 0 ; p != p_detail.length ; p++ ){
      pixel_content +=  p + ".: " + p_detail[p] + "\n";
      sum += p_detail[p]; 
    }
    if( option == 1 ){
      pixel_content += "sum: " + sum;
    }
    fill(255);
    
    rect(  mouseX + 5 , mouseY + 10 , 200 , 200);
    fill(0); 
    text( pixel_content  , mouseX + 20 , mouseY + 20 );
    
    stroke( 255 , 0 , 0 );
    writePixels( i + (int)maps.xmin + 1 , j  + (int)maps.ymin + 1 , 20 );
  }else{
    stroke(255,0,255);
    fill(255,255,0);
    writePixels( mouseX/20 , mouseY/20 , 20 );
  }
    String method_name = ""; 
    switch( option ){
      case 1: method_name = "Winding NumberI"; break;
      case 2: method_name = "Half sapce"; break;
      case 3: method_name = "Winding NumberII"; break;
    }
    textSize( 32 );
    textAlign( CENTER , CENTER );
    stroke( 127,127,255);
    fill(168,0,0,168);
    rect( 0 , 0 , width , 16 );
    fill(0,255,0);
    text( method_name , width/2 , 8 );
    fill(0,0,168,168);
    rect( 0 , height - 16 , width , height );
    fill(168,0,168);
    textAlign( CENTER , BOTTOM );
    text( method_name , width/2 , height );
    textSize( 16 );
    
  
  fill( 255 , 0 , 255);
  //rect( mouseX + 10 , mouseY + 30 , t.length() * 16 , 20 ); 
  stroke(0);
}

public void setup() {
  frameRate(30);
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
public void writeLine( int x, int y ,int x1 , int y1 , String tag , int fc , boolean accept){
   x = 20*x + 10;   y = 20*y + 10; 
  x1 = 20*x1 + 10; y1 = 20*y1 + 10;
  strokeWeight( 2 );
  stroke( fc );
  line( x , y , x1 , y1 ); 
  
  float center_x = 0.5f*x + 0.5f*x1;
  float center_y = 0.5f*y + 0.5f*y1;
  
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

public void writePixels( int x, int y, float r ) {
  ellipse(  x*r + 0.5f*r, y*r + 0.5f*r, r, r );
}

public void ScreenPixelLabels(int cols, int rows, float r) {
  textAlign(LEFT);
  for ( int i = 0 ; i != cols ; i++ )
    text( i, r + i*r, r );
  textAlign(CENTER);
  for ( int i = 0 ; i != rows ; i++ )
    text( i, 0.5f*r, 2*r + i*r );
}
public void ScreenPixels(int cols, int rows, float r) {
  fill( 255);
  float hr = 0.5f*r;
  for ( int i = 0 ; i != cols ; i++ )
    for ( int j = 0 ; j != rows ; j++ )
      ellipse( i*r + hr, j*r + hr, r, r );
}


public void windingNumberTestI( ArrayList<PVector> pts){
  if( pts.size() < 3 )return;
  findBoundary( pts , maps );
  maps.setMode( HalfSpaceMap.WINDING_I ); 
  float angle[] = new float[pts.size()];
 
  /* winding num tests*/
   for( int y = (int)maps.ymin ; y <= (int)maps.ymax ; y++ ){
    for( int x = (int)maps.xmin ; x <= (int)maps.xmax ; x++ ){
      PVector point = new PVector( x , y ); 
      for( int i = 0 ; i != pts.size() ; i++){
        PVector p0 = new PVector( pts.get(i).x , pts.get(i).y );
        PVector p1 = new PVector( pts.get( (i+1)%pts.size() ).x , pts.get( (i+1)%pts.size() ).y );
        
        p0.sub( point );
        p1.sub( point );
        
        p0.normalize();
        p1.normalize();
        
       angle[i] = acos( p0.dot( p1 ) );
      }
      maps.add( angle );
    }
   }
}

public boolean isIntersetion(PVector line0_p0 , PVector line0_p1 , PVector line1_p0 , PVector line1_p1 ){
  float x1,x2,x3,x4;
  float y1,y2,y3,y4;
  
  x1 = line0_p0.x; x2 = line0_p1.x;
  x3 = line1_p0.x; x4 = line1_p1.x;
  
  y1 = line0_p0.y; y2 = line0_p1.y;
  y3 = line1_p0.y; y4 = line1_p1.y;
  
  float d = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
  if( d > 1e-10f || d < -1e-10f){
    PVector p = new PVector( (x1*y2 - y1*x2)*(x3-x4) - (x1-x2)*(x3*y4 - y3*x4) / d , (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4 - y3*x4) / d);
    float min_line0x = min( line0_p0.x , line0_p1.x );
    float max_line0x = max( line0_p0.x , line0_p1.x ); 
    float min_line0y = min( line0_p0.y , line0_p1.y );
    float max_line0y = max( line0_p0.y , line0_p1.y );
    if( p.x >= min_line0x && p.x <= max_line0x && p.y >= min_line0y && p.y <= max_line0y )
      return true;
  }
  return false;
}
public void windingtNumberTestII( ArrayList<PVector> pts ){
  if( pts.size() < 3 )return;
  findBoundary( pts , maps );
  maps.setMode( HalfSpaceMap.WINDING_II );
   for( int y = (int)maps.ymin ; y <= (int)maps.ymax ; y++ ){
    for( int x = (int)maps.xmin ; x <= (int)maps.xmax ; x++ ){
      for( int i = 0 ; i < pts.size() - 2 ; i++ ){
      //select two line
      PVector p0 = pts.get(i);
      PVector p1 = pts.get(i + 1);
      PVector p2 = pts.get(i + 2);
      PVector p3 = pts.get((i + 3) % pts.size() );
      PVector n = new PVector();
      float en[] = new float[3];
      if( isIntersetion( p0 , p1 , p2 , p3 ) ){
        en[0] = lineEq(p0,p1,x,y,n);
        en[1] = lineEq(p1,p2,x,y,n);
        en[2] = lineEq(p2,p3,x,y,n);
        maps.add( en );
      }
        //continue from next line
        /*for( int j = (i  + 1) % pts.size() ; j != 0 ;  ++j%pts.size() ){
          
        }*/
      }  
    }
   }
  //return false;
}

public void findBoundary( ArrayList<PVector> pts , HalfSpaceMap map ){
   float xmin = 1e+30f;
   float ymin = 1e+30f;
  
   float xmax = 1e-30f;
   float ymax = 1e-30f;
  
   for( int i = 0 ; i != pts.size() ; i++ ){
     xmin = min( xmin , pts.get(i).x );
     ymin = min( ymin , pts.get(i).y );
    
     xmax = max( xmax , pts.get(i).x );
     ymax = max( ymax , pts.get(i).y );
   }
   map.xmin = xmin; map.ymin = ymin;
   map.xmax = xmax; map.ymax = ymax;
}

public float lineEq( PVector p0 , PVector p1 , float x ,float y , PVector n){
  PVector p = new PVector(p0.x , p0.y);
  p.sub(p1);
  p.normalize();
  n.x = p.y; n.y = -p.x;
  float c = -( n.x * p0.x + n.y * p0.y ); 
  return n.x * x + n.y * y + c; 
}
public void half_space_draw( ArrayList<PVector> pts ){
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
      
      PVector line_center = new PVector( (pts.get(i0).x + pts.get(i1).x)*0.5f , (pts.get(i0).y + pts.get(i1).y)*0.5f );
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
public void HalfSpaceTest(  ArrayList<PVector> pts ){
  /* half space tests*/
  findBoundary( pts , maps );
  maps.setMode( HalfSpaceMap.HALFSPACE );
  float xDim =  maps.xmax - maps.xmin + 1;
  float en[] = new float[pts.size()];
  PVector n[] = new PVector[pts.size()];
  for( int i = 0 ; i != pts.size() ; i++ ){
    n[i] = new PVector();
    en[i] = lineEq( pts.get(i) , pts.get( (i+1)%pts.size() ) , maps.xmin , maps.ymin , n[i]);
  }
  
   for( int y = (int)maps.ymin ; y <= (int)maps.ymax ; y++ ){
    for( int x = (int)maps.xmin ; x <= (int)maps.xmax ; x++ ){
      maps.add( en );
      for( int e_num = 0 ; e_num != pts.size() ; e_num++ )
        en[ e_num ] += n[e_num].x;
    }
    for( int e_num = 0 ; e_num != pts.size() ; e_num++ ) 
     en[ e_num ] += -xDim*n[e_num].x + n[e_num].y; 
   }
}

//Assume pts are already in order CCW / CW
public void fill_triangle( ArrayList<PVector> pts){
 
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
public void draw_line(int Px0 , int Py0 , int Px1 , int Py1 ) {
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


public void draw() {
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
  
  for(int i = 0 ; i != triangle.size() ; i++ ){
     stroke(0);
     fill(255);
     writePixels(  (int)triangle.get(i).x , (int)triangle.get(i).y , 20 );
  }
  for(int i = 0 ; i != triangle.size(); i++){
     draw_line( (int)triangle.get(i).x , (int)triangle.get(i).y , (int)triangle.get( (i+1)%triangle.size() ).x , (int)triangle.get( (i+1)%triangle.size() ).y );
  }
  if( triangle.size() > 1){
    int size = triangle.size();
    boolean b_accept[] = getAcceptMarker(size);
    for(int i = 0 ; i != size ; i++ ){
       writeLine( (int)triangle.get(i).x , (int)triangle.get(i).y ,  (int)triangle.get( (i+1)%size ).x  , (int)triangle.get( (i+1)%size ).y , "e" + i , color( 255 * (i&0x04) , 255 * (i&0x02) , 255 * (i&0x01) ) , b_accept[i] );
    }
  }  

  stroke(0);
   
  popMatrix();
  //testCase07();
  //testCase16();
  showTextMenu();
  
}
public void runMethod( int index ){
    maps.clear();
    switch( index ){
      case 1: windingNumberTestI(triangle); break;
      case 2: HalfSpaceTest(triangle); break;
      case 3: windingtNumberTestII(triangle); break;
    }
}
public void keyPressed(){
  if( key == 'x'){
    option = (option + 1)%3 + 1;
    runMethod(option);
    redraw();
  }
  if( key == 's')
    ss -= 0.1f;
  if( key == 'S')
    ss += 0.1f;
  if( ss < 0.1f ) ss = 0.1f;
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
int option = 3;
public void mousePressed() {
  if( mouseButton == LEFT ){
    /*
    if( triangle.size() >= 3 ){
      triangle.clear();
      maps.clear();
    }
    triangle.add( new PVector( (mouseX - 20)/20 , (mouseY - 20)/20 ) );
    if( triangle.size() >= 3 ) fill_triangle( triangle );
    */
    triangle.add( new PVector( (mouseX - 20)/20 , (mouseY - 20)/20 ) );
    runMethod( option );
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

public void testCase07() {
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

public void testCase16() {
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

public void testCase34() {
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

public void testCase25() {
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

  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "polygon_rasterization" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
