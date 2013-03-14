#include "ObjObject.h"
#include <cmath>

ObjObject::ObjObject(void)
{
	n = fn = 0;
	minv[0] = minv[1] = minv[2] = 1e37;
	maxv[0] = maxv[1] = maxv[2] =  1e-37;
	scale_x = scale_y = scale_z = 1.0;
}


ObjObject::~ObjObject(void)
{
}

vector<POINT> *ObjObject::getPoints(){
	return &pts;
}
vector<FACEINDEX> *ObjObject::getFaces(){
	return &faces;
}

int ObjObject::parseText( const char* txt ){
	const char* p = txt;
	/*
	while( *p != '\n' ){
		if( *p == 'v')
			return OBJ_ADD_POINT;
		if( *p == 'f')
			return OBJ_COMPOSITE_FACE;
		p++;
	}*/
		if( *p == 'v' && *(p+1) == ' ')
			return OBJ_ADD_POINT;
		if( *p == 'f' && *(p+1) == ' ')
			return OBJ_COMPOSITE_FACE;
	return 0;
}

const char* ObjObject::readline( FILE* fin ){
	char c;
	char* start = buffer;
	while( (c = fgetc( fin )) != '\n' && c != EOF ){
		*start = c;
		start++;
	}
	if( c == EOF )*start = EOF;
	else *start = '\0';
	return buffer;
}
void ObjObject::ToUnit(){
	scale_x = (maxv[0] - minv[0]);
	scale_y = (maxv[1] - minv[1]);
	scale_z = (maxv[2] - minv[2]);
	float px = max( max( scale_x , scale_y ) , scale_z);
	scale_x /= px;
	scale_y /= px;
	scale_z /= px;
	for(size_t i = 0 ; i != pts.size() ; i++){
		pts[i].x = scale_x * ((pts[i].x - minv[0])/(maxv[0] - minv[0]) -0.5 );
		pts[i].y = scale_y *((pts[i].y - minv[1])/(maxv[1] - minv[1]) -0.5 );
		pts[i].z = scale_z *((pts[i].z - minv[2])/(maxv[2] - minv[2]) -0.5 );
	}

}
void ObjObject::loadFile( const char* filename ){
	FILE* fin = fopen( filename , "r" );
	if( fin == NULL ){
		cout << "file not found" << endl;
		return;
	}
	const char* txt;
	float tx,ty,tz;
	vector<int> tindexs;
	POINT tmp;
	FACEINDEX tmp_findex;
	while( (txt = readline(fin)) && *txt != EOF ){
		switch( parseText( txt ) ){
		case OBJ_ADD_POINT:
			getPoint( txt , &tx , &ty , &tz );
			//push point
			tmp.x = tx;
			tmp.y = ty;
			tmp.z = tz;
			pts.push_back( tmp );
			break;
		case OBJ_COMPOSITE_FACE:
			getFace( txt , &tindexs );
			if( tindexs.size() >= 3 ){
				tmp_findex.index[0] = tindexs[0];
				tmp_findex.index[1] = tindexs[1];
				tmp_findex.index[2] = tindexs[2];
				faces.push_back( tmp_findex );
			}
			//push face
			break;
		}
	}	
}

void ObjObject::getPoint(const char* txt , float* px , float *py , float *pz){
	//std::cout << txt << endl;
	sscanf( ++txt , "%f%f%f" , px , py , pz );
	if( *px < minv[0] ) minv[0] = *px;
	if( *py < minv[1] ) minv[1] = *py;
	if( *pz < minv[2] ) minv[2] = *pz;

	if( *px > maxv[0] ) maxv[0] = *px;
	if( *py > maxv[1] ) maxv[1] = *py;
	if( *pz > maxv[2] ) maxv[2] = *pz;
	n++;
}

void ObjObject::getFace(const char* txt , vector<int>* indexs ){
	indexs->clear();
	//std::cout << txt << endl;
	int index = 0;
	int index_1 = 0;
	int index_2 = 0;
	
	sscanf( ++txt , "%d%d%d" ,  &index , &index_1 , &index_2 );
	indexs->push_back( index - 1);
	indexs->push_back( index_1 - 1);
	indexs->push_back( index_2 - 1);
	fn++;
}

void ObjObject::testValue( int n , int fn ){
	
	if( n != this->n )
		cout << "Error points count is not same. real n:" << this->n  << endl;
	if( fn != this->fn )
		cout << "Error face count is not same. real fn:" << this->fn<< endl;
	if( n == this->n && fn == this->fn )
		cout << "result: correct~ :) " << endl;
		

}

void ObjObject::showBoundingBox(){
	float minX,minY,minZ;
	float maxX,maxY,maxZ;
	minX = scale_x * minv[0];
	minY = scale_y * minv[1];
	minZ = scale_z * minv[2];
	
	maxX = scale_x * maxv[0];
	maxY = scale_y * maxv[1];
	maxZ = scale_z * maxv[2];

	glBegin(GL_LINE_LOOP);
	glVertex3f( minX , minY , minZ );
	glVertex3f( minX , minY , maxZ );
	glVertex3f( maxX , minY , maxZ );
	glVertex3f( maxX , minY , minZ );

	glBegin(GL_LINE_LOOP);
	glVertex3f( minX , minY , minZ );
	glVertex3f( minX , minY , minZ );
	glVertex3f( minX , minY , minZ );
	glVertex3f( minX , minY , minZ );


	glVertex3f( minX , minY , minZ );
	glVertex3f( minX , minY , minZ );
	glVertex3f( minX , minY , minZ );
	glVertex3f( minX , minY , minZ );
	glEnd();
}