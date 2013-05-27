
#include "MainGL.h"


MainGL::~MainGL(void)
{
}

void MainGL::draw(){
	if( valid() ){
		glViewport(0, 0, w(), h());	
		glMatrixMode(GL_PROJECTION);	//��v�x�}
		glLoadIdentity();				//��l�Ưx�}��
		gluPerspective(60.0, (GLfloat)w()/(GLfloat)h(), 0.1, 10.0);	
	
		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();
	
		gluLookAt(0,0,1.5,
				  0,0,-1,
				  0,1,0);
		/*gluLookAt(�۾�����m,
					�۾��ݪ���V,
					�۾����k�V�q)*/	
	}

}
