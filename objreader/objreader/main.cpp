#include <stdlib.h>
#include <stdio.h> 
#include <GL/glut.h>
#include <iostream>
#include "ObjObject.h"
using namespace std;

/*該程式會用到的頂點位置和顏色資訊*/
//我有三個頂點，且每個頂點有三個座標值x,y,z.
GLfloat Vertex_pos[5][3] = {{2.5f, 0.0f, 2.5f},							
							{-2.5f, 0.0f, 2.5f},														
							{2.5f, 0.0f, -2.5f},
							{-2.5f, 0.0f, -2.5f},
							{0.0f, 5.0f, 0.0f}};
GLfloat Point_color[] = { 1.0f, 0.0f, 0.0f};
GLfloat Line_color[] = { 0.0f, 0.0f, 0.0f};
GLfloat Face_color[] = { 0.0f, 0.0f, 1.0f};

//顯示文字在畫面上的function ‧參考資源 http://www.youtube.com/watch?v=elE__Nouv54
string pyramid_status_text = "Pyramid status - (Normal)";
string cullface_status_text = "CullFace status - (Disabled)";
string frontface_text = "glFrontFace - (GL_CCW)";
string cullface_text = "glCullFace - (GL_BACK)";

ObjObject obj;
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
	MENU_BOTTOMPLANE_DISVISIBLE,
	MENU_CULL_FACE_ON,
	MENU_CULL_FACE_OFF,
	MENU_CULL_BACKFACE,			
	MENU_CULL_FRONTFACE,
	MENU_CULL_FRONT_AND_BACKFACE,			
	MENU_FRONTFACE_CCW,
	MENU_FRONTFACE_CW,
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
	
	gluLookAt(0,0,1.5,
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	vector<POINT>* pPts = obj.getPoints();
	vector<FACEINDEX>* pFace = obj.getFaces();


	//顯示頂點POINTS	
	glPointSize(5.0f);
	glPushMatrix();
	//glTranslatef( -0.5,-0.5,-0.5);
	glBegin(GL_POINTS);	
		for( size_t i = 0 ; i != (*pPts).size(); i++ ){
			glVertex3f( (*pPts)[i].x , (*pPts)[i].y , (*pPts)[i].z );
		}
	glEnd();
		
	//顯示線條LINES
	glLineWidth(1.0);
	glEnable( GL_POLYGON_OFFSET_LINE );
	glPolygonOffset( -1.5 , -1.5 );
	glBegin(GL_LINES);
		glColor3fv(Line_color);
		for( size_t i = 0 ; i != (*pFace).size() ; i++){
			glVertex3f( (*pPts)[(*pFace)[i].index[2]].x , (*pPts)[(*pFace)[i].index[2]].y , (*pPts)[(*pFace)[i].index[2]].z );
			glVertex3f( (*pPts)[(*pFace)[i].index[1]].x , (*pPts)[(*pFace)[i].index[1]].y , (*pPts)[(*pFace)[i].index[1]].z );
		
			glVertex3f( (*pPts)[(*pFace)[i].index[1]].x , (*pPts)[(*pFace)[i].index[1]].y , (*pPts)[(*pFace)[i].index[1]].z );
			glVertex3f( (*pPts)[(*pFace)[i].index[0]].x , (*pPts)[(*pFace)[i].index[0]].y , (*pPts)[(*pFace)[i].index[0]].z );
		
			glVertex3f( (*pPts)[(*pFace)[i].index[0]].x , (*pPts)[(*pFace)[i].index[0]].y , (*pPts)[(*pFace)[i].index[0]].z );
			glVertex3f( (*pPts)[(*pFace)[i].index[2]].x , (*pPts)[(*pFace)[i].index[2]].y , (*pPts)[(*pFace)[i].index[2]].z );
			
		}
	glEnd();
	glDisable( GL_POLYGON_OFFSET_LINE );
	glBegin(GL_TRIANGLES);
		for( size_t i = 0 ; i != (*pFace).size() ; i++){
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3f( (*pPts)[(*pFace)[i].index[2]].x , (*pPts)[(*pFace)[i].index[2]].y , (*pPts)[(*pFace)[i].index[2]].z );
			glColor3f(1.0f,0.0f,1.0f);	
			glVertex3f( (*pPts)[(*pFace)[i].index[1]].x , (*pPts)[(*pFace)[i].index[1]].y , (*pPts)[(*pFace)[i].index[1]].z );
			glColor3f(0.0f,0.0f,1.0f);	
			glVertex3f( (*pPts)[(*pFace)[i].index[0]].x , (*pPts)[(*pFace)[i].index[0]].y , (*pPts)[(*pFace)[i].index[0]].z );
		}
	glEnd();
	glPopMatrix();
	/*
	//顯示文字訊息
	glColor3f(0.0f,0.0f,0.0f);	
	drawText(pyramid_status_text.data(),pyramid_status_text.size(),10,120);
	drawText(cullface_status_text.data(),cullface_status_text.size(),10,90);
	drawText(frontface_text.data(),frontface_text.size(),10,60);	
	drawText(cullface_text.data(),cullface_text.size(),10,30);
	*/
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
	isBottomPlaneVisible = true;
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
}

void reset(){
	glEnable(GL_DEPTH_TEST);	
	isBottomPlaneVisible = true;
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	pyramid_status_text = "Pyramid status - (Normal)";
	cullface_status_text = "CullFace status - (Disabled)";
	frontface_text = "glFrontFace - (GL_CCW)";
	cullface_text = "glCullFace - (GL_BACK)";
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

void menu(int item){	//主要參考 http://stackoverflow.com/questions/14370/glut-pop-up-menus
	switch(item)
	{
		case MENU_RESET:
			reset();
			break;		
		case MENU_BOTTOMPLANE_DISVISIBLE:
			isBottomPlaneVisible = false;
			pyramid_status_text = "Pyramid status - (bottom inVisible)";
			break;	
		case MENU_CULL_FACE_ON:
			glEnable(GL_CULL_FACE);
			cullface_status_text = "CullFace status - (Enabled)";
			break;	
		case MENU_CULL_FACE_OFF:
			glDisable(GL_CULL_FACE);
			cullface_status_text = "CullFace status - (Disabled)";
			break;	
		case MENU_CULL_BACKFACE:
			glCullFace(GL_BACK);
			cullface_text = "glCullFace - (GL_BACK)";
			break;	
		case MENU_CULL_FRONTFACE:
			glCullFace(GL_FRONT);
			cullface_text = "glCullFace - (GL_FRONT)";
			break;	
		case MENU_CULL_FRONT_AND_BACKFACE:
			glCullFace(GL_FRONT_AND_BACK);
			break;	
		case MENU_FRONTFACE_CCW:
			glFrontFace(GL_CCW);
			frontface_text = "glFrontFace - (GL_CCW)";
			break;	
		case MENU_FRONTFACE_CW:
			glFrontFace(GL_CW);
			frontface_text = "glFrontFace - (GL_CW)";
			break;	
	}
	glutPostRedisplay();
}

/*****************************************************
				主要GLUT程式進入點
******************************************************/

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	const char* default_file = "teapot(3).obj";
	if( argc >= 2 )default_file = argv[1];
	obj.loadFile(default_file);
	obj.ToUnit();
	Init();		

	//創造menu
	glutCreateMenu(menu);
	//增加menu item
	glutAddMenuEntry("Reset",MENU_RESET);	
	glutAddMenuEntry("Pyramid - remove bottom plane",MENU_BOTTOMPLANE_DISVISIBLE);		
	glutAddMenuEntry("Enable(CullFace)",MENU_CULL_FACE_ON);
	glutAddMenuEntry("Disable(CullFace)",MENU_CULL_FACE_OFF);	
	glutAddMenuEntry("CullFace(Back)",MENU_CULL_BACKFACE);	
	glutAddMenuEntry("CullFace(Front)",MENU_CULL_FRONTFACE);	
	glutAddMenuEntry("CullFace(Front and Back)",MENU_CULL_FRONT_AND_BACKFACE);	
	glutAddMenuEntry("FrontFace(counterclockwise)",MENU_FRONTFACE_CCW);
	glutAddMenuEntry("FrontFace(clockwise)",MENU_FRONTFACE_CW);	
	//連接滑鼠按鈕來使用menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(My_SpecialKeys);
	glutKeyboardFunc(My_Keyboard);
	glutMainLoop();
	return 0;
}
