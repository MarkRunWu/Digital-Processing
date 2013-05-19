#include <stdlib.h>
#include <stdio.h> 
#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

typedef struct{
	float x;
	float y;
	float z;
}myVec3;
vector<myVec3> mesh;
vector<myVec3> subdividedMesh;
vector<myVec3> tmpMesh;
GLenum MODE = GL_LINES;
bool b_auto_rotate = false;
bool b_hidden_point = false;
bool b_normalize = false;
static float angles;
static int sub_depth;
//lighting settings
GLfloat light_ambient[] = {0.2 , 0.0 , 0.2 , 1.0 };
GLfloat light_diffuse[] = {0.2 , 0.2 , 0.2 , 1.0 };
GLfloat light_specular[] = {1 , 1 , 1 , 1.0 };
GLfloat light_global_ambient[] = {0.2 , 0.2 , 0.2 , 1.0 };

//define a polygon
#define X .525731112119133606
#define Z .850650808352039932
static GLfloat vdata[12][3]={
{-X,0.0,Z},{X,0.0,Z},{-X,0.0,-Z},{X,0.0,-Z},
{0.0,Z,X},{0.0,Z,-X},{0.0,-Z,X},{0.0,-Z,-X},
{Z,X,0.0},{-Z,X,0.0},{Z,-X,0.0},{-Z,-X,0.0}
};
static GLuint tindices[20][3]={ 
{1,4,0},{4,9,0},{4,9,5},{8,5,4},{1,8,4},
{1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
{3,10,7},{10,6,7},{6,11,7},{6,0,11},{6,1,0}, 
{10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7}
}; 


//顯示文字在畫面上的function ‧參考資源 http://www.youtube.com/watch?v=elE__Nouv54

string line_text = "GL_LINE_MODE:  ";
string fill_text = "GL_FILL_MODE:  ";
string auto_rotate_text = "AutoRotate:  ";
string point_text = "Hidden points:  ";
string normalize_text = "Normalize during subdivid:  ";
string subdivid_count_text = "SubDiv Depth:  ";
void normalize( myVec3 *noob_v ){
	myVec3 v = *noob_v;
	float l = sqrt( v.x*v.x + v.y*v.y + v.z*v.z );
	if( l != 0 ){
		v.x /= l;
		v.y /= l;
		v.z /= l;
	}
	*noob_v = v;
}
void subdividmesh( myVec3 v0, myVec3 v1 , myVec3 v2 , int depth ){
	if( depth == 0 ){
		tmpMesh.push_back( v0 );
		tmpMesh.push_back( v1 );
		tmpMesh.push_back( v2 );
		return;
	}
	myVec3 mid01, mid12, mid20;
	mid01.x = 0.5*v0.x + 0.5*v1.x; mid12.x = 0.5*v1.x + 0.5*v2.x; mid20.x = 0.5*v2.x + 0.5*v0.x;
	mid01.y = 0.5*v0.y + 0.5*v1.y; mid12.y = 0.5*v1.y + 0.5*v2.y; mid20.y = 0.5*v2.y + 0.5*v0.y;
	mid01.z = 0.5*v0.z + 0.5*v1.z; mid12.z = 0.5*v1.z + 0.5*v2.z; mid20.z = 0.5*v2.z + 0.5*v0.z;
	if( b_normalize ){
		normalize( &mid01 );
		normalize( &mid12 );
		normalize( &mid20 );
	}
	subdividmesh( v0 , mid01 , mid20 , depth - 1 );
	subdividmesh( mid01 , v1 , mid12 , depth - 1 );
	subdividmesh( mid20 , mid12 , v2 , depth - 1 );
	subdividmesh( mid01 , mid12 , mid20 , depth - 1 );
}

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
void draw_mesh( GLenum mode , vector<myVec3> mesh ){
	glColor3ub(255,0,0);
	glBegin( mode );
	switch( mode ){
	case GL_LINES:
		for(int i = 0 ; i != mesh.size() ; i+=3 ){
			glVertex3f( mesh[i].x , mesh[i].y , mesh[i].z );	glVertex3f( mesh[i + 1].x , mesh[i + 1].y , mesh[i + 1].z );
			glVertex3f( mesh[i + 1].x, mesh[i + 1].y , mesh[i + 1].z );	glVertex3f( mesh[i + 2].x , mesh[i + 2].y , mesh[i + 2].z );
			glVertex3f( mesh[i + 2].x, mesh[i + 2].y , mesh[i + 2].z ); glVertex3f( mesh[i].x , mesh[i].y , mesh[i].z );	
		}
		break;
	case GL_TRIANGLES:
		for(int i = 0 ; i != mesh.size() ; i+=3 ){
			glNormal3f( mesh[i].x , mesh[i].y , mesh[i].z );
			glVertex3f( mesh[i].x , mesh[i].y , mesh[i].z );
			glNormal3f( mesh[i + 1].x, mesh[i + 1].y , mesh[i + 1].z );
			glVertex3f( mesh[i + 1].x, mesh[i + 1].y , mesh[i + 1].z );
			glNormal3f( mesh[i + 2].x, mesh[i + 2].y , mesh[i + 2].z );
			glVertex3f( mesh[i + 2].x, mesh[i + 2].y , mesh[i + 2].z );
		}
		break;
	}
	glEnd();
	
	if( !b_hidden_point ){
		glBegin( GL_POINTS );
		for(int i = 0 ; i != mesh.size() ; i+=3 ){
				glVertex3f( mesh[i].x , mesh[i].y , mesh[i].z );
				glVertex3f( mesh[i + 1].x, mesh[i + 1].y , mesh[i + 1].z );
				glVertex3f( mesh[i + 2].x, mesh[i + 2].y , mesh[i + 2].z );
			}
		glEnd();
	}
}
/*菜單的型別列舉*/
enum MENU_TYPE	//供選單切換時使用
{
	MENU_RESET,
	MENU_LINE_MODE,	
	MENU_FILL_MODE,
	MENU_AUTO_ROTATE,
	MENU_HIDDEN_POINT,
	MENU_NORMALIZE_SUBDIVID
};
MENU_TYPE show;	//先宣告準備使用
bool isBottomPlaneVisible;	//是否顯示底部

/*****************************************************
			當視窗被創造/被調整大小時執行
******************************************************/
GLdouble cube_size = 15;
void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);	
	glMatrixMode(GL_PROJECTION);	//投影矩陣
	glLoadIdentity();				//初始化矩陣值
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);	
	
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	
	gluLookAt(0,0,5.5,
			  0,0,-1,
			  0,1,0);
			  
	/*gluLookAt(相機的位置,
				相機看的方向,
				相機的法向量)*/	
}

/*****************************************************
			       主要處理繪圖動作
******************************************************/
void RenderScene(void)
{// Clear the window with current clearing color
	if( b_auto_rotate ){
		++angles;
		if( angles > 360 ) angles = 0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );			
	
	
	//顯示頂點POINTS	
	glPointSize(5.0f);

	glShadeModel( GL_FLAT );
	glPushMatrix();
	glTranslatef( -3 , 0 , 0);
	glRotatef( angles , 0,0,1);
	draw_mesh( MODE , mesh );
	glPopMatrix();
	
	glShadeModel( GL_SMOOTH );
	glPushMatrix();
	glRotatef( angles , 0,0,1);
	draw_mesh( MODE ,  mesh );
	glPopMatrix();

	glPushMatrix();
	glTranslatef( 3 , 0 , 0);
	glRotatef( angles , 0,0,1);
	glColor3ub( 255,0,0);
	draw_mesh( MODE , subdividedMesh );
	glPopMatrix();
	
	//顯示文字訊息
	glColor3f(0.0f,0.0f,0.0f);

	string output_txt = "";
	output_txt = subdivid_count_text;
	output_txt += '0' + sub_depth;
	drawText(output_txt.data(),output_txt.size(),10,160);
	output_txt = line_text;
	output_txt += MODE == GL_LINES ? "Enable" : "Disable";
	drawText(output_txt.data(),output_txt.size(),10,130);
	output_txt = fill_text;
	output_txt += MODE == GL_TRIANGLES ? "Enable" : "Disable";
	drawText(output_txt.data(),output_txt.size(),10,100);
	output_txt = auto_rotate_text;
	output_txt += b_auto_rotate ? "Enable" : "Disable";
	drawText(output_txt.data(),output_txt.size(),10,70);
	output_txt = point_text;
	output_txt += b_hidden_point ? "Enable" : "Disable";
	drawText(output_txt.data(),output_txt.size(),10,40);
	output_txt = normalize_text;
	output_txt += b_normalize ? "Enable" : "Disable";
	drawText(output_txt.data(),output_txt.size(),10,10);
	glutSwapBuffers(); // Perform the buffer swap to display the back buffer
}

/*****************************************************
			       初始化/預先啟用功能
******************************************************/
void Init(){	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize(900, 600);
	glutCreateWindow("mini hw lighting/normal/subdivide 100598020");
	
	glEnable(GL_DEPTH_TEST);	
	//isBottomPlaneVisible = true;
	/* test polygon
	myVec3 p0,p1,p2,p3,p4,p5;
	p0.x = 50;	p0.y = 150; p0.z = 0;
	p1.x = -50; p1.y = 50;	p1.z = 0;
	p2.x = -50; p2.y = -100;p2.z = 0;
	p3.x = 50;	p3.y = -150;p3.z = 0;
	p4.x = 140; p4.y = -20; p4.z = 0;
	p5.x = 0;	p5.y = 0;	p5.z = 0;
	mesh.push_back(p0 );mesh.push_back(p1 );mesh.push_back(p5 ); 
	mesh.push_back(p1 );mesh.push_back(p2 );mesh.push_back(p5); 
	mesh.push_back(p2);mesh.push_back(p3);mesh.push_back(p5); 
	mesh.push_back(p3);mesh.push_back(p4);mesh.push_back(p5); 
	mesh.push_back(p4);mesh.push_back(p0);mesh.push_back(p5);
	*/
	myVec3 tmp;
	for(int i = 0 ; i != 20 ; i++ ){
		for(int p = 0 ; p != 3 ; p++ ){
			tmp.x = vdata[ tindices[i][p] ][0];
			tmp.y = vdata[ tindices[i][p] ][1];
			tmp.z = vdata[ tindices[i][p] ][2];
			mesh.push_back( tmp );
		}
	}
	subdividedMesh = mesh;

	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	glLightfv( GL_LIGHT0 , GL_AMBIENT , light_ambient );
	glLightfv( GL_LIGHT0 , GL_DIFFUSE , light_diffuse );
	glLightfv( GL_LIGHT0 , GL_SPECULAR , light_specular );

	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHTING );
	glEnable( GL_COLOR_MATERIAL );
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
			glRotatef(1.0,0,0,1);  //依照Z軸旋轉  
			break;
		case GLUT_KEY_RIGHT:
			glRotatef(-1.0,0,0,1);              
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
			break;
		case '+':
			sub_depth = min( sub_depth + 1, 10);
			tmpMesh.clear();
			for( size_t i = 0 ; i != mesh.size() ; i+=3 ){
				subdividmesh( mesh[i] , mesh[i+1] , mesh[i+2] , sub_depth );
			}
			subdividedMesh = tmpMesh;
			break;
		case '-':
			sub_depth = max( sub_depth - 1 , 0 );
			tmpMesh.clear();
			for( size_t i = 0 ; i != mesh.size() ; i+=3 ){
				subdividmesh( mesh[i] , mesh[i+1] , mesh[i+2] , sub_depth );
			}
			subdividedMesh = tmpMesh;
			break;
	}
	glutPostRedisplay();
}

void menu(int item){	//主要參考 http://stackoverflow.com/questions/14370/glut-pop-up-menus
	switch(item)
	{
		case MENU_RESET:
			subdividedMesh = mesh;
			break;
		case MENU_LINE_MODE:
			MODE = GL_LINES;
			break;		
		case MENU_FILL_MODE:
			MODE = GL_TRIANGLES;
			break;	
		case MENU_AUTO_ROTATE:
			b_auto_rotate ^= 1;
			break;
		case MENU_HIDDEN_POINT:
			b_hidden_point ^= 1;
			break;
		case MENU_NORMALIZE_SUBDIVID:
			b_normalize ^= 1;
			break;
	}
	glutPostRedisplay();
}
void idle(){
	glutPostRedisplay();
}

/*****************************************************
				主要GLUT程式進入點
******************************************************/

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	Init();		

	//創造menu
	glutCreateMenu(menu);
	//增加menu item
	glutAddMenuEntry("-------shade part---------",-1);
	glutAddMenuEntry("polygon-LINE_MODE",MENU_LINE_MODE);	
	glutAddMenuEntry("polygon-FILL_MODE",MENU_FILL_MODE);		
	glutAddMenuEntry("Hidden points", MENU_HIDDEN_POINT);
	glutAddMenuEntry("-----subdivid part--------",-1);
	glutAddMenuEntry("Normalize subdivid", MENU_NORMALIZE_SUBDIVID);
	glutAddMenuEntry("--------------------------",-1);
	glutAddMenuEntry("polygon-Auto rotation",MENU_AUTO_ROTATE );
	glutAddMenuEntry("Reset mesh", MENU_RESET);
	
	//連接滑鼠按鈕來使用menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(My_SpecialKeys);
	glutKeyboardFunc(My_Keyboard);
	glutIdleFunc(idle );
	glutMainLoop();
	return 0;
}
