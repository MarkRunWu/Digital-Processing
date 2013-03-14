#pragma once
#include <iostream>
#include <vector>
#include <GL/glut.h>
#define OBJ_ADD_POINT 11
#define OBJ_COMPOSITE_FACE 13
using namespace std;

typedef struct{
	float x;
	float y;
	float z;
}POINT;
typedef struct{
	int index[3];
}FACEINDEX;
class ObjObject
{
public:
	ObjObject(void);
	~ObjObject(void);
	void loadFile( const char* filename );
	void testValue( int n , int fn);
	vector<POINT> *getPoints(); 
	vector<FACEINDEX>* getFaces();
	void ToUnit();
	void showBoundingBox();
private:
	const char* readline( FILE* fin);
	int parseText( const char* text );
	void getPoint(const char* txt ,  float *x , float *y , float *z );
	void getFace(const char* txt , vector<int>* indexs );
	char buffer[512];
	int n;
	int fn;
	vector<POINT> pts;
	vector<FACEINDEX> faces;
	float minv[3];
	float maxv[3];
	float scale_x;
	float scale_y;
	float scale_z;
};

