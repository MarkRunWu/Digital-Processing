#include "ObjObject.h"
//openMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <LinearSystemLib.h>
typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

MyMesh mesh;
void initMesh( obj::ObjObject* pObj){
	vector<obj::POINT>* pts = pObj->getPoints();
	vector<obj::FACEINDEX>* fs = pObj->getFaces();

	MyMesh::VertexHandle* vhandler = new MyMesh::VertexHandle[pts->size()];
	for( size_t i = 0 ; i != pts->size() ; i++ ){
		vhandler[i] = mesh.add_vertex( MyMesh::Point( (*pts)[ i ].x , (*pts)[ i ].y , (*pts)[ i ].z ) );
	}

	vector<MyMesh::VertexHandle> face_handler;
	for( size_t i = 0 ; i != fs->size() ; i++ ){
		face_handler.clear();
		face_handler.push_back( vhandler[(*fs)[i].index[0]] );
		face_handler.push_back( vhandler[(*fs)[i].index[1]] );
		face_handler.push_back( vhandler[(*fs)[i].index[2]] );
		mesh.add_face( face_handler );
	}

	delete [] vhandler;
}


void deleteVertex( MyMesh::VertexHandle h ){
	vector<MyMesh::VertexHandle> vs;
	for( MyMesh::VVIter v = mesh.vv_iter( h ) ; v  ; ++v ){
		vs.push_back( v );
	}	

	mesh.request_vertex_status();
	mesh.request_face_status();
	mesh.request_edge_status();
	cout << "v count: " <<  vs.size() << endl;
	mesh.delete_vertex( h , false );
	mesh.garbage_collection();

	vector<MyMesh::VertexHandle> vhandles;
	
	for( size_t i = 1 ; i < vs.size() - 1 ; i++ ){
		vhandles.clear();
		if( !mesh.status( vs[0]).deleted() && !mesh.status( vs[i]).deleted() && !mesh.status( vs[i+1]).deleted() ){
			vhandles.push_back( vs[0] );
			vhandles.push_back( vs[i+1] );
			vhandles.push_back( vs[i] );
			mesh.add_face( vhandles );
		}
	}
	mesh.release_vertex_status();
	mesh.release_face_status();
	mesh.release_edge_status();
}

void SolveLinearSystem(MyMesh* mesh,std::vector<int> controlPoint_indexes){
	if(mesh==NULL)
	{
		return ;
	}
	//Ax=B
	LinearSystemLib::GeneralSparseMatrix GA;//Ax=B 的 A
	double** B_X = new double*[1];//Ax=B 的 B 針對x座標解
	double** B_Y = new double*[1];//Ax=B 的 B 針對y座標解
	double** B_Z = new double*[1];//Ax=B 的 B 針對z座標解
	double** solution_X = 0;//Ax=B x的解 針對x座標解
	double** solution_Y = 0;//Ax=B x的解 針對y座標解
	double** solution_Z = 0;//Ax=B x的解 針對z座標解
	//
	int n_vertices = mesh->n_vertices();//Mesh's total vertex
	int n_control_point = controlPoint_indexes.size();//total control point
	//Set matrix size
	GA.Create(n_vertices+n_control_point, n_vertices);
	B_X[0] = new double[n_vertices+n_control_point];
	B_Y[0] = new double[n_vertices+n_control_point];
	B_Z[0] = new double[n_vertices+n_control_point];
	//填上半部矩陣 點對點
	for(MyMesh::VIter v_it = mesh->vertices_begin(); v_it!=mesh->vertices_end(); ++v_it)
	{
		MyMesh::VHandle vh = v_it.handle();
		int row,col;//要放入的r,c
		int vv_total = 0;//點對點的總量
		double element;//要放入矩陣的元素

		row = v_it.handle().idx();//Set row index
		GA.SetElement(row,row,1);//if row == column , set 1 to (row,column)

		//先獲得點對點總量
		for(MyMesh::VVIter vv_it = mesh->vv_iter(v_it.handle());vv_it;++vv_it)
		{
			vv_total++;
		}
		//要放入矩陣的元素 為 -1/vv_total
		element = double(-1) / double(vv_total);
		//放入矩陣的元素
		for(MyMesh::VVIter vv_it = mesh->vv_iter(v_it.handle());vv_it;++vv_it)
		{
			col = vv_it.handle().idx();//Set column index
			GA.SetElement(row,col,element);//Set element
			//printf("row : %d , col : %d , element : %f\n",row,col,element);
		}

		//Set B's element 上半部為0
		B_X[0][row] = 0.0;
		B_Y[0][row] = 0.0;
		B_Z[0][row] = 0.0;
	}
	//Control point
	for(int i=0;i<controlPoint_indexes.size();i++)
	{
		//填下半部矩陣 control point
		GA.SetElement(n_vertices+i,controlPoint_indexes[i],1);
		//Set control point to B's 下半部
		MyMesh::VHandle vh = mesh->vertex_handle(controlPoint_indexes[i]);
		B_X[0][ n_vertices+i ] = mesh->point(vh)[0];
		B_Y[0][ n_vertices+i ] = mesh->point(vh)[1];
		B_Z[0][ n_vertices+i ] = mesh->point(vh)[2];
	}
	//Solve linear system
	LinearSystemLib::SparseLinearSystem sls_X(new LinearSystemLib::StableSparseMatrix(GA), B_X, 1);
	LinearSystemLib::SparseLinearSystem sls_Y(new LinearSystemLib::StableSparseMatrix(GA), B_Y, 1);
	LinearSystemLib::SparseLinearSystem sls_Z(new LinearSystemLib::StableSparseMatrix(GA), B_Z, 1);
	try
	{
		bool result;
		result = LinearSystemLib::LeastSquareSparseLSSolver::GetInstance()->Solve( &sls_X, solution_X );
		result = LinearSystemLib::LeastSquareSparseLSSolver::GetInstance()->Solve( &sls_Y, solution_Y );
		result = LinearSystemLib::LeastSquareSparseLSSolver::GetInstance()->Solve( &sls_Z, solution_Z );
	} 
	catch( std::exception e )
	{
		printf("Error : %s",e.what());
		//cerr << e.what() << endl;
		return ;
	}
	//Set point to mesh
	for(int i=0;i<n_vertices;i++)
	{
		MyMesh::VHandle vh = mesh->vertex_handle(i);
		mesh->point(vh)[0] =  solution_X[0][i];
		mesh->point(vh)[1] =  solution_Y[0][i];
		mesh->point(vh)[2] =  solution_Z[0][i];
	}

	printf("Solve success");
}


