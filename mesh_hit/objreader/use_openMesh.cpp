#include "ObjObject.h"
//openMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

MyMesh mesh;
void initMesh( ObjObject* pObj){
	vector<POINT>* pts = pObj->getPoints();
	vector<FACEINDEX>* fs = pObj->getFaces();
	MyMesh::VertexHandle vhandler[3];
	
	vector<MyMesh::VertexHandle> face_handler;
	for( size_t i = 0 ; i != fs->size() ; i++ ){
		face_handler.clear();
		vhandler[0] = mesh.add_vertex( MyMesh::Point( (*pts)[ (*fs)[i].index[0] ].x , (*pts)[ (*fs)[i].index[0] ].y , (*pts)[ (*fs)[i].index[0] ].z ) );
		vhandler[1] = mesh.add_vertex( MyMesh::Point( (*pts)[ (*fs)[i].index[1] ].x , (*pts)[ (*fs)[i].index[1] ].y , (*pts)[ (*fs)[i].index[1] ].z ) );
		vhandler[2] = mesh.add_vertex( MyMesh::Point( (*pts)[ (*fs)[i].index[2] ].x , (*pts)[ (*fs)[i].index[2] ].y , (*pts)[ (*fs)[i].index[2] ].z ) );
		
		face_handler.push_back( vhandler[0] );
		face_handler.push_back( vhandler[1] );
		face_handler.push_back( vhandler[2] );
		mesh.add_face( face_handler );
	}

}