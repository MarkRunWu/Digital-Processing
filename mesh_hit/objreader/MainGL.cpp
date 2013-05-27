
#include "MainGL.h"


MainGL::~MainGL(void)
{
}

void MainGL::draw(){
	if( valid() ){
		glViewport(0, 0, w(), h());	
		glMatrixMode(GL_PROJECTION);	//投影矩陣
		glLoadIdentity();				//初始化矩陣值
		gluPerspective(60.0, (GLfloat)w()/(GLfloat)h(), 0.1, 10.0);	
	
		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();
	
		gluLookAt(0,0,1.5,
				  0,0,-1,
				  0,1,0);
		/*gluLookAt(相機的位置,
					相機看的方向,
					相機的法向量)*/	
	}

}
