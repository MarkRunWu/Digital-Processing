#include <stdlib.h>
#include <stdio.h> 
#include <GL/glut.h>
#include <iostream>
#include "ObjObject.h"

using namespace std;


#include "common.h"

extern MyMesh mesh;
extern void initMesh(obj::ObjObject*);
extern void deleteVertex( MyMesh::VertexHandle h );
extern void SolveLinearSystem(MyMesh* mesh,std::vector<int> controlPoint_indexes);
obj::ObjObject model;
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


typedef struct{
	float x;
	float y;
	float z;
}CVector3;

CVector3 mouse3D;
vector<MyMesh::VertexHandle> visiblePoints;
vector<MyMesh::FaceHandle> visibleFaces;

//reference http://nehe.gamedev.net/article/using_gluunproject/16013/
CVector3 GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 
	CVector3 p;{
		p.x = posX;
		p.y = posY;
		p.z = posZ;
	}
    return p;
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
	MENU_RANDOM_DELETE_VERTEXS
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
	
	
	//顯示頂點POINTS	
	glPointSize(5.0f);
	glPushMatrix();
	//glTranslatef( -0.5,-0.5,-0.5);
	//obj.draw();
	//obj.showBoundingBox();
    glColor3ub( 255,255,0);
    bool b_hit;	
	glBegin( GL_TRIANGLES );
	for( MyMesh::FaceIter ft =  mesh.faces_begin() ; ft != mesh.faces_end() ; ++ft ){
		b_hit = false;
		for( size_t i = 0 ; i != visibleFaces.size() ; i++ ){
			if( visibleFaces[i] == ft ){
				b_hit = true;
				break;
			}
		}
		if( b_hit ){
			glColor3ub( 255, 0, 0);
		}else{
			glColor3ub( 255,255,0);
		}
		for( MyMesh::FaceVertexIter vt = mesh.fv_iter( ft.handle() ) ; vt ; ++vt ){
			glVertex3fv( mesh.point( vt.handle() ).data() );
		}	
	}
	glEnd();

	glColor3ub( 255 ,0 ,0);
	float* p;
	glBegin( GL_POINTS );
	for( MyMesh::VertexIter vt  = mesh.vertices_begin() ; vt != mesh.vertices_end() ; ++vt ){
		b_hit = false;
		for( size_t i = 0 ; i != visiblePoints.size() ; i++ ){
			if( mesh.point( vt.handle() ).data() == mesh.point( visiblePoints[i] ).data() ){
				b_hit = true;
				break;
			}
		}
		if( b_hit ) glColor3ub( 255 , 0 , 0 );
		else glColor3ub( 255 , 255 , 255 );
		p = mesh.point( vt.handle() ).data();
		glVertex3f( p[0] , p[1] , p[2] );
	}
	glEnd();

	float*p1,*p2;
	glColor3ub( 0 , 0 , 255 );
	glBegin( GL_LINES );
	for( MyMesh::EdgeIter he = mesh.edges_begin()  ; he != mesh.edges_end() ; ++he ){
		p1 = mesh.point( mesh.to_vertex_handle( mesh.halfedge_handle( he , 0 ) ) ).data();
		p2 = mesh.point( mesh.from_vertex_handle( mesh.halfedge_handle( he , 0 ) ) ).data();
		glVertex3f( p1[0] , p1[1] , p1[2] );
		glVertex3f( p2[0] , p2[1] , p2[2] );
	}
	glEnd();

	//float*p1,*p2,*p3;
	

	glPopMatrix();
	/*
	//顯示文字訊息
	glColor3f(0.0f,0.0f,0.0f);	
	drawText(pyramid_status_text.data(),pyramid_status_text.size(),10,120);
	drawText(cullface_status_text.data(),cullface_status_text.size(),10,90);
	drawText(frontface_text.data(),frontface_text.size(),10,60);	
	drawText(cullface_text.data(),cullface_text.size(),10,30);
	*/
	glBegin(GL_POINTS);
	glVertex3f( mouse3D.x , mouse3D.y , mouse3D.z );
	glEnd();
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

void make_edge( float e[4] , float p0[3] , float p1[3] , float orthV[3] ){
	float tmp[3];
	tmp[0] = p1[0] - p0[0];
	tmp[1] = p1[1] - p0[1];
	tmp[2] = p1[2] - p0[2];
	
	e[0] = tmp[1]*orthV[2] - tmp[2]*orthV[1];
	e[1] = tmp[2]*orthV[0] - tmp[0]*orthV[2];
	e[2] = tmp[0]*orthV[1] - tmp[1]*orthV[0];
	e[3] = e[0] * p1[0] + e[1] * p1[1] + e[2] * p1[2];
}
void mouse( int button , int state , int x , int y ){
	cout << mesh.n_vertices() << endl;
	vector<int> pths;
	//for(int i = 0 ; i != 10 ; i++ )
	pths.push_back( 1 );
	float* pos = mesh.point( mesh.vertex_handle( 1 ) ).data();
	cout << pos[0] << "," << pos[1] << "," << pos[2] << endl;
	SolveLinearSystem( &mesh , pths );
	/*
	float* pos;
	 for( MyMesh::VertexIter vt =  mesh.vertices_begin() ; vt != mesh.vertices_end() ; ++vt ){
		pos = mesh.point( vt.handle() ).data();
		if( mouse3D.x - pos[0] < 1e-2 && mouse3D.x - pos[0] > -1e-2 && mouse3D.y - pos[1] < 1e-2 && mouse3D.y - pos[1] > -1e-2 && mouse3D.z - pos[2] < 1e-2 && mouse3D.z - pos[2] > -1e-2 ){
			deleteVertex( vt.handle() );
			break;
		}
	 }
	 */
}
void mouse_move(int x, int y){
	bool b_found = false;
	visiblePoints.clear();
	visibleFaces.clear();
	cout << "mouse move " << x << "," << y << endl;
	mouse3D = GetOGLPos( x , y );

	//select point ?
	float* pos,*posEnd;
	for( MyMesh::VertexIter vt =  mesh.vertices_begin() ; vt != mesh.vertices_end() ; ++vt ){
		pos = mesh.point( vt.handle() ).data();
		if( mouse3D.x - pos[0] < 1e-2 && mouse3D.x - pos[0] > -1e-2 && mouse3D.y - pos[1] < 1e-2 && mouse3D.y - pos[1] > -1e-2 && mouse3D.z - pos[2] < 1e-2 && mouse3D.z - pos[2] > -1e-2 ){
			for( MyMesh::VertexVertexIter vv = mesh.vv_iter( vt.handle() ) ; vv ; ++vv ){
				visiblePoints.push_back( vv.handle() );
			}
			cout << visiblePoints.size() << endl;
			b_found = true;
			break;
		}
	}
	//select edge ?
	if( !b_found ){
		float* p1,*p2;
		float v1[3],v2[3];
		float l1,l2;
		for( MyMesh::EdgeIter he = mesh.edges_begin()  ; he != mesh.edges_end() ; ++he ){
			p1 = mesh.point( mesh.to_vertex_handle( mesh.halfedge_handle( he , 0 ) ) ).data();
			p2 = mesh.point( mesh.from_vertex_handle( mesh.halfedge_handle( he , 0 ) ) ).data();

			v1[0] = (mouse3D.x - p1[0]);
			v2[0] = (mouse3D.x - p2[0]);
			v1[1] = (mouse3D.y - p1[1]);
			v2[1] = (mouse3D.y - p2[1]);
			v1[2] = (mouse3D.z - p1[2]);
			v2[2] = (mouse3D.z - p2[2]);
			l1 = sqrt( v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2] );
			l2 = sqrt( v2[0]*v2[0] + v2[1]*v2[1] + v2[2]*v2[2] );
			v1[0] /= l1; v1[1] /= l1; v1[2] /= l1;
			v2[0] /= l2; v2[1] /= l2; v2[2] /= l2;

			if( v1[0] + v2[0] < 1e-1 && v1[0] + v2[0] > -1e-1 &&
				v1[1] + v2[1] < 1e-1 && v1[1] + v2[1] > -1e-1 &&
				v1[2] + v2[2] < 1e-1 && v1[2] + v2[2] > -1e-1){
					visibleFaces.push_back( mesh.face_handle( mesh.halfedge_handle( he , 0 ) ) );
					visibleFaces.push_back( mesh.opposite_face_handle( mesh.halfedge_handle( he , 0 ) ) );
				b_found = true;
				break;
			}
		}
	}
	//glutPostRedisplay();
	//return;

	float *p1,*p2,*p3;
	float v1[4],v2[4],v3[4],e1[4],e2[4],e3[4];

	if( !b_found ){
	   //select face ?
		for( MyMesh::FaceIter f = mesh.faces_begin() ; f != mesh.faces_end() ; ++f ){
			MyMesh::FaceVertexIter vp = mesh.fv_iter( f );
			p1 = mesh.point( vp ).data(); ++vp;
			p2 = mesh.point( vp ).data(); ++vp;
			p3 = mesh.point( vp ).data();
			v1[0] = p2[0] - p1[0];
			v1[1] = p2[1] - p1[1];
			v1[2] = p2[2] - p1[2];
			//v2[0] = p3[0] - p2[0]; v2[1] = p3[1] - p2[1]; v2[2] = p3[2] - p2[2];
			v3[0] = p3[0] - p1[0];
			v3[1] = p3[1] - p1[1];
			v3[2] = p3[2] - p1[2];

			v2[0] = v1[1]*v3[2] - v1[2]*v3[1];
			v2[1] = v1[2]*v3[0] - v1[0]*v3[2];
			v2[2] = v1[0]*v3[1] - v1[1]*v3[0];
			v2[3] = (v2[0] * p1[0] + v2[1] * p1[1] + v2[2] * p1[2]);
			make_edge( e1 , p1 , p2 , v2 );
			make_edge( e2 , p2 , p3 , v2 );
			make_edge( e3 , p3 , p1 , v2 );
			float t  = v2[0] * mouse3D.x + v2[1] * mouse3D.y + v2[2] * mouse3D.z - v2[3];
			int c_e1 = ( e1[0] * mouse3D.x + e1[1] * mouse3D.y + e1[2] * mouse3D.z - e1[3] ) > 0 ? 1 : -1;
			int c_e2 = ( e2[0] * mouse3D.x + e2[1] * mouse3D.y + e2[2] * mouse3D.z - e2[3] ) > 0 ? 1 : -1;
			int c_e3 = ( e3[0] * mouse3D.x + e3[1] * mouse3D.y + e3[2] * mouse3D.z - e3[3] ) > 0 ? 1 : -1;

			if( c_e1 == c_e2 && c_e1 == c_e3  && t > -1e-3  && t < 1e-3 ){
				cout << (c_e1 == c_e2) << "," << (c_e1 == c_e3) << endl;
				MyMesh::FaceVertexIter vp = mesh.fv_iter( f );
				visiblePoints.push_back( vp );vp++;
				visiblePoints.push_back( vp );vp++;
				visiblePoints.push_back( vp );
				for( MyMesh::FaceFaceIter ff = mesh.ff_iter( f ) ; ff ; ++ff ){
					visibleFaces.push_back( ff.handle() );
				}
				//break;
			}
		}
	}
	glutPostRedisplay();
}

void random_del_vertex( int count ){
	while( count-- ){
		int num = rand()%mesh.n_vertices() + 1;
		cout << num << endl;
		deleteVertex( mesh.vertex_handle( num ) );
	}
}

void menu(int item){	//主要參考 http://stackoverflow.com/questions/14370/glut-pop-up-menus
	switch(item)
	{
		case MENU_RESET:
			reset();
			break;	
		case MENU_RANDOM_DELETE_VERTEXS:
			random_del_vertex(100);
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
	const char* default_file = "../../teddy.obj";
	if( argc >= 2 )default_file = argv[1];
	model.loadFile(default_file);
	model.ToUnit();
	initMesh( &model );
	Init();		
	//創造menu
	glutCreateMenu(menu);
	//增加menu item
	glutAddMenuEntry("Reset",MENU_RESET);	
	glutAddMenuEntry("randomly remove verties",MENU_RANDOM_DELETE_VERTEXS);		
	//連接滑鼠按鈕來使用menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(My_SpecialKeys);
	glutKeyboardFunc(My_Keyboard);
	glutPassiveMotionFunc( mouse_move );
	glutMouseFunc( mouse );
	glutMainLoop();
	return 0;
}
