int begin_pt[] = new int[2];
int end_pt[] = new int[2];
int count = 0;
void setup(){
  size(500,500);
}
void writePixels( int x , int y , float r ){
  fill( 255,255,0);
  ellipse(  x*r + 0.5*r , y*r + 0.5*r , r , r );
  fill( 255);
}
void ScreenPixels(int cols , int rows , float r){
  float hr = 0.5*r;
  for( int i = 0 ; i != cols ; i++ )
    for( int j = 0 ; j != rows ; j++ )
      ellipse( i*r + hr , j*r + hr , r , r );
}
void draw_line(int p0[] , int p1[]){
  //dx
  //dy
  //d
  //init d
  //if d > 0 ne , +a
  //if d < 0 e , +a+b
  //..
  /*** y = mx + b 
  -> dx*y = dy*x + b 
  -> dx*y - dy*x - b = 0
  */
  int dy = p1[1] - p0[1];
  int dx = p1[0] - p0[0];
  
  int d = 2*dy - dx;
  
  int delE = 2*dy;
  int delNE = 2*(dy - dx);
  int step = 1;
  
  if(dy < 0){
    delE = -2*dy;
    delNE = -2*( dy + dx);
    d = -2*dy - dx;
    step = -1;
  }
  int tx,ty;
  tx = p0[0];
  ty = p0[1];
  
  int condition = p1[0];
  boolean reverse = false;
  if( dy > dx ){
    int tmp = ty;
    ty = tx;
    tx = tmp;
    condition = p1[1];
    reverse = true;
  }
  
  if( reverse ) writePixels(ty,tx,20);
   else writePixels(tx,ty,20);
   
  while( tx < condition  ){
    if( d > 0 ){ //NE
      d += delNE;
      ty+=step;   
    }else{ // E
      d += delE;
    }
    tx++;
    if( reverse ) writePixels(ty,tx,20);
    else writePixels(tx,ty,20);
  }
}
void draw(){
  background(127);
  ScreenPixels( 25 , 25 , 20 );
  
  if( count > 0 ){
    writePixels( begin_pt[0] , begin_pt[1] , 20 );
    if( count > 1){
      writePixels( end_pt[0] , end_pt[1] , 20 );
      draw_line( begin_pt , end_pt );
    }
  }
}
void mousePressed(){
  if( count >= 2 )count = 0;
  if( count == 0 ){
   begin_pt[0] = mouseX;
   begin_pt[1] = mouseY;
   begin_pt[0] /= 20;
   begin_pt[1] /= 20;
  }else{
    end_pt[0] = mouseX;
    end_pt[1] = mouseY;
   end_pt[0] /= 20;
   end_pt[1] /= 20;
  }
  count++;
}
