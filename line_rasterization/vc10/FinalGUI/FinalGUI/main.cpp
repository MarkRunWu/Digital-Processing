#include <stdlib.h>
#include <stdio.h> 
#include <GL/glut.h>
#include <iostream>
#include <fstream>

using namespace std;

//problem : center intersection cal?

//顯示文字在畫面上的function ‧參考資源 http://www.youtube.com/watch?v=elE__Nouv54
string antialiasing_text;// = "Anti-aliasing: (Disable)";
string log_file_text;
//string rest_text = "Reset";
//string frontface_text = "glFrontFace - (GL_CCW)";
//string cullface_text = "glCullFace - (GL_BACK)";


#define GRID_W 20
#define GRID_H 20
typedef struct {
	int x;
	int y;
	bool valid;
}SCREEN_POINT;

SCREEN_POINT start_pts, end_pts;
//last data
float _offset = 0;
float _scale = 1;
int _button;
int _x;
int _y;
int _w , _h;

bool b_antialiasing = false;
bool b_exporting_log = false;
bool b_show_menu = false;
bool b_show_grid = true;
bool b_log_file = false;
void checkMenu();
void drawGrid(int w , int h); // draw a grid start from (0,0)  with w and h size
extern void MyLine(int Px0 , int Py0 , int Px1 , int Py1 , bool );
extern void MyPoint(int Px0 , int Py0 );
fstream log_file;



void drawText(const char *text,int length,int x,int y){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();		
		glLoadIdentity();
		glOrtho(0,800,0,600,-5,5);	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glRasterPos2i(x,y);
			for(int i=0;i<length;i++){
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)text[i]);
			}
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);	//要有一個矩陣去接PopMatrix值
	glPopMatrix();				
	glMatrixMode(GL_MODELVIEW);
}

/*菜單的型別列舉*/
enum MENU_TYPE	//供選單切換時使用
{
	MENU_RESET,	
	MENU_ANTIALIASING,
	MENU_SHOW_TEXT,
	MENU_SHOW_GRID,
	MENU_LOG_FILE
};
MENU_TYPE show;	//先宣告準備使用

/*****************************************************
			當視窗被創造/被調整大小時執行
******************************************************/
GLdouble cube_size = 15;
void ChangeSize(int w, int h)
{
	_w = w; _h = h;
	glViewport(0, 0, w, h);	
	glMatrixMode(GL_PROJECTION);	//投影矩陣
	glLoadIdentity();				//初始化矩陣值
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	gluOrtho2D( 0 , GRID_W , GRID_H , 0 );
}

void drawGrid(int w , int h){
	glBegin( GL_LINES );
		for( int i = 0 ; i <= w ; i++ )
			for( int j = 0 ; j <= h ; j++ ){
				glVertex2i( i , 0 );glVertex2i( i , h );
				glVertex2i( 0 , j );glVertex2i( w , j );
			}
	glEnd();
}

/*****************************************************
			       主要處理繪圖動作
******************************************************/
void RenderScene(void)
{// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	
	
	//顯示頂點POINTS

	glColor3f(0.0f,0.0f,0.0f);	

	glPushMatrix();
	//glTranslatef( _offset , _offset , 0 );
	glScalef( _scale , _scale , 1 );
	//glTranslatef( -0.5,-0.5,-0.5);
	if( start_pts.valid)
		MyPoint( start_pts.x , start_pts.y );
	if( end_pts.valid )
		MyPoint( end_pts.x , end_pts.y );
	if( start_pts.valid && end_pts.valid )
		MyLine( start_pts.x , start_pts.y , end_pts.x , end_pts.y , false ); 
	
	glColor3f(0.0f,0.0f,0.0f);
	if( b_show_grid )
		drawGrid( GRID_W , GRID_H ); 
	glPopMatrix();
	
	
	
	
	


	//顯示文字訊息
	if( b_show_menu ){
		drawText(antialiasing_text.data(),antialiasing_text.size(),10,120);
		drawText(log_file_text.data() , log_file_text.size(),10,150);
	}
		
	
	glutSwapBuffers(); // Perform the buffer swap to display the back buffer
}

/*****************************************************
			       初始化/預先啟用功能
******************************************************/
void Init(){	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("obj reader 100598020");
	
	glEnable(GL_DEPTH_TEST);
	checkMenu();
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	start_pts.valid = false;
	end_pts.valid = false;
}

void reset(){
	b_antialiasing = false;
	b_exporting_log = false;
	b_show_grid = true;
	b_show_menu = false;
	start_pts.valid = false;
	end_pts.valid = false;
	b_log_file = false;
	_scale = 1;
	_offset = 0;
}

/*****************************************************
			  特殊鍵的事件處理(像是方向鍵)
******************************************************/
void My_SpecialKeys(int key,int x,int y){	
	switch(key){
		case GLUT_KEY_UP:			
			glRotatef(1.0,1,0,0);  //依照X軸旋轉     
			break;
		case GLUT_KEY_DOWN:
			glRotatef(-1.0,1,0,0);                   
			break;
		case GLUT_KEY_LEFT:
			glRotatef(1.0,0,1,0);  //依照Z軸旋轉  
			break;
		case GLUT_KEY_RIGHT:
			glRotatef(-1.0,0,1,0);              
			break;
		case GLUT_KEY_F1:
			glTranslatef(0,0,2);
			break;
		case GLUT_KEY_F2:
			glTranslatef(0,0,-2);
			break;
	}
	glutPostRedisplay(); //重新繪製圖像
}
void My_Keyboard(unsigned char key,int x,int y){
	switch(key){		
		case 27:	//press ESC to exit program.
			exit(0);			
	}		
}

void checkMenu(){
	antialiasing_text = b_antialiasing ? "Anti-aliasing: (Enable)" : "Anti-aliasing: (Disable)" ;
	log_file_text = b_log_file ? "Recording: (Enable)" : "Recording: (Disable)" ;
}

void menu(int item){	//主要參考 http://stackoverflow.com/questions/14370/glut-pop-up-menus
	switch(item)
	{
		case MENU_RESET:
			reset();
			break;		
		case MENU_ANTIALIASING:
			b_antialiasing ^= 1;
			break;
		case MENU_SHOW_TEXT:
			b_show_menu ^= 1;
			break;
		case MENU_SHOW_GRID:
			b_show_grid ^= 1;
			break;
		case MENU_LOG_FILE:
				log_file.open( "log.txt" , fstream::out );
				MyLine( start_pts.x , start_pts.y , end_pts.x , end_pts.y , true );
				log_file.close();
			
	}
	checkMenu();
	glutPostRedisplay();
}
#define MAX( x , y )( (x) > (y) ? (x) : (y) )
#define MIN( x , y )( (x) < (y) ? (x) : (y) )
bool mouse_combo = false;
void mouse_drag( int x , int y ){
	if( GLUT_LEFT_BUTTON == _button){
		mouse_combo = false;
		_scale += ( x - _x ) / 100.f;
		_scale = MAX( _scale , 0.1 );
		_offset = GRID_W*0.5 - GRID_W*0.5*_scale;
	}
	_x = x;
	glutPostRedisplay();
}
void mouse( int button , int state , int x , int y ){
	switch( button ){
	case GLUT_LEFT_BUTTON:
		if( state == GLUT_UP ){
			if( mouse_combo ){
				float step_x = _scale*_w/GRID_W;
				float step_y = _scale*_h/GRID_H;
				
				int ix = x/step_x;
				int iy = y/step_y;
				if( end_pts.valid ){
					start_pts.valid = false;
					end_pts.valid = false;
				}
				if( !start_pts.valid ){
					start_pts.x = ix;
					start_pts.y = iy;
					start_pts.valid = true;
				}else{
					end_pts.x = ix;
					end_pts.y = iy;
					end_pts.valid = true;
				}
				mouse_combo = false;
			}
		}
		if( state == GLUT_DOWN ){
			mouse_combo = true;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	}
	_button = button;
	_x = x;
	glutPostRedisplay();
}

/*****************************************************
				主要GLUT程式進入點
******************************************************/

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	const char* default_file = "../../teapot.obj";
	if( argc >= 2 )default_file = argv[1];
	
	Init();		

	//創造menu
	glutCreateMenu(menu);
	//增加menu items
	glutAddMenuEntry("Anti-aliasing",MENU_ANTIALIASING);
	glutAddMenuEntry("Show Text ",MENU_SHOW_TEXT);
	glutAddMenuEntry("Show Grid",MENU_SHOW_GRID);
	glutAddMenuEntry("Log file" , MENU_LOG_FILE );
	glutAddMenuEntry("Reset",MENU_RESET);
	//連接滑鼠按鈕來使用menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMouseFunc( mouse );
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(My_SpecialKeys);
	glutKeyboardFunc(My_Keyboard);
	glutMotionFunc( mouse_drag );
	glutMainLoop();
	return 0;
}
