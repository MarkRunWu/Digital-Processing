#pragma once
#ifndef _GUA_OM_H_
#define _GUA_OM_H_
#undef min
#undef max
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Utils/getopt.h>

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

//#include "GLCamera.h"

struct Face_InnerAngle
{
	double Vertex_Angle[3];
};

class AllPairHarVal_class
{
public:  
	std::vector<double> Har_val;
};

namespace OMT//OpenMesh Triangle mesh
{
	using namespace std;
	/*----------------------------------------------------------------------*/

	/*©w¸q¨Ï¥Îªººë·Ç«×©M°ò¥»ÄÝ©Ê*/
	struct MyTraits : OpenMesh::DefaultTraits
	{
		// let Point and Normal be a vector made from doubles
		typedef OpenMesh::Vec3d Point;
		typedef OpenMesh::Vec3d Normal;

		// add normal property to vertices and faces
		VertexAttributes(OpenMesh::Attributes::Normal);
		FaceAttributes  (OpenMesh::Attributes::Normal);

		// Already defined in OpenMesh::DefaultTraits
		// HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );

		// Uncomment next line to disable attribute PrevHalfedge
		// HalfedgeAttributes( OpenMesh::Attributes::None );
		//
		// or
		//
		// HalfedgeAttributes( 0 );
	};
	/*----------------------------------------------------------------------*/

	/*©w¸q±`¥Îtype*/
	typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>	    MyMesh	;
	typedef OpenMesh::Vec3d									Vector3d;	//Vec3D type
	typedef MyMesh::Scalar									Scalar	;	//Scalar type
	typedef MyMesh::Point									Point	;	//Point type
	typedef MyMesh::Normal									Normal	;	//Normal type
	typedef MyMesh::VertexHandle							VHandle	;	//VertexHandle type
	typedef MyMesh::HalfedgeHandle							HEHandle;	//HalfedgeHandle type
	typedef MyMesh::EdgeHandle							    EHandle ;	//edgeHandle type
	typedef MyMesh::FaceHandle								FHandle	;	//FaceHandle type
	//-------------Vertex iterators & circulators-------------
	typedef MyMesh::VertexIter								VIter	;	//VertexIter type
	typedef MyMesh::VertexVertexIter						VVIter	;	//VertexVertexIter type
	typedef MyMesh::VertexEdgeIter							VEIter	;	//VertexEdgeIter type
	typedef MyMesh::VertexFaceIter							VFIter	;	//VertexFaceIter type
	typedef MyMesh::EdgeIter								EIter	;	//EdgeIterT	type
	typedef MyMesh::FaceIter								FIter	;	//FaceIter type
	typedef MyMesh::FaceVertexIter							FVIter	;	//FaceVertexIter type
	typedef MyMesh::FaceEdgeIter							FEIter	;	//FaceEdgeIter type
	typedef MyMesh::FaceFaceIter							FFIter	;	//FaceFaceIter type
	typedef MyMesh::VertexOHalfedgeIter						VOHEIter;	//VertexOutHalfEdge type
	typedef MyMesh::ConstVertexVertexIter					CVVIter	;	//ConstVertexVertexIter type
	/*----------------------------------------------------------------------*/

	/*©w¸qÃB¥~¸ê®Æµ²ºc*/
	using namespace OpenMesh;
	struct sp_f 
	{
		FHandle fh;
		float r, g, b;
	};
	//«ü©w¯S§Oµe¥X³»ÂIªº¸ê®Æµ²ºc
	struct sp_v 
	{
		VHandle vh;
		float r, g, b;
	};
	//«ü©w¥t¥~µe¥X¦ì¸mªº¸ê®Æµ²ºc
	struct sp_p
	{
		Point pt;
		float r, g, b;
	};
	//«ü©w¥t¥~µe¥XÃäªº¸ê®Æµ²ºc
	struct sp_e
	{
		EHandle eh;
		float r, g, b;
	};
	
	/*----------------------------------------------------------------------*/

	/*©w¸qÃþ§O*/
	class Model:public MyMesh
	{
	public:
		Model();//constructor
		~Model();//de-constructor
		MyMesh Mesh;	
		void RenderSpecifiedPoint();	//µe¥X«ü©w¦ì¸mªºÂI
		void RenderSpecifiedVertex();	//µe¥X«ü©wªº³»ÂI
		void RenderSpecifiedFace();		//µe¥X«ü©wªº­±
		void RenderSpecifiedEdge();		//µe¥X«ü©wªºÃä


		void add_sp_p(Point   _p, float _r, float _g, float _b);//«ü©wÃB¥~µe¥XªºÂI
		void add_sp_v(VHandle _v, float _r, float _g, float _b);//«ü©wÃB¥~µe¥Xªº³»ÂI
		void add_sp_f(FHandle _f, float _r, float _g, float _b);//«ü©wÃB¥~µe¥Xªº­±
		void add_sp_e(VHandle   _p1, VHandle   _p2, float _r, float _g, float _b);//«ü©wÃB¥~µe¥XªºÃä

		void clear_sp_p();//²MªÅÃB¥~µe¥XªºÂI
		void clear_sp_v();//²MªÅÃB¥~µe¥Xªº³»ÂI
		void clear_sp_f();//²MªÅÃB¥~µe¥Xªº­±
		void clear_sp_e();//²MªÅÃB¥~µe¥XªºÃä

	public:
		vector< sp_p > sp_p_list;
		vector< sp_v > sp_v_list;
		vector< sp_f > sp_f_list;
		vector< sp_v > sp_e_list;
	}; 
}
/*======================================================================*/

namespace OMP//OpenMesh Polygonal mesh
{
	using namespace std;
	/*----------------------------------------------------------------------*/

	/*©w¸q¨Ï¥Îªººë·Ç«×©M°ò¥»ÄÝ©Ê*/
	struct MyTraits : OpenMesh::DefaultTraits
	{
		// let Point and Normal be a vector made from doubles
		typedef OpenMesh::Vec3d Point;
		typedef OpenMesh::Vec3d Normal;

		// add normal property to vertices and faces
		VertexAttributes(OpenMesh::Attributes::Normal);
		FaceAttributes  (OpenMesh::Attributes::Normal);

		// Already defined in OpenMesh::DefaultTraits
		// HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );

		// Uncomment next line to disable attribute PrevHalfedge
		// HalfedgeAttributes( OpenMesh::Attributes::None );
		//
		// or
		//
		// HalfedgeAttributes( 0 );
	};
	/*----------------------------------------------------------------------*/

	/*©w¸q±`¥Îtype*/
	typedef OpenMesh::PolyMesh_ArrayKernelT<MyTraits>	    MyMesh	;
	typedef OpenMesh::Vec3d									Vector3d;	//Vec3D type
	typedef MyMesh::Scalar									Scalar	;	//Scalar type
	typedef MyMesh::Point									Point	;	//Point type
	typedef MyMesh::Normal									Normal	;	//Normal type
	typedef MyMesh::VertexHandle							VHandle	;	//VertexHandle type
	typedef MyMesh::HalfedgeHandle							HEHandle;	//HalfedgeHandle type
	typedef MyMesh::EdgeHandle							    EHandle ;	//edgeHandle type
	typedef MyMesh::FaceHandle								FHandle	;	//FaceHandle type
	//-------------Vertex iterators & circulators-------------
	typedef MyMesh::VertexIter								VIter	;	//VertexIter type
	typedef MyMesh::VertexVertexIter						VVIter	;	//VertexVertexIter type
	typedef MyMesh::VertexEdgeIter							VEIter	;	//VertexEdgeIter type
	typedef MyMesh::VertexFaceIter							VFIter	;	//VertexFaceIter type
	typedef MyMesh::EdgeIter								EIter	;	//EdgeIterT	type
	typedef MyMesh::FaceIter								FIter	;	//FaceIter type
	typedef MyMesh::FaceVertexIter							FVIter	;	//FaceVertexIter type
	typedef MyMesh::FaceEdgeIter							FEIter	;	//FaceEdgeIter type
	typedef MyMesh::FaceFaceIter							FFIter	;	//FaceFaceIter type
	typedef MyMesh::VertexOHalfedgeIter						VOHEIter;	//VertexOutHalfEdge type
	typedef MyMesh::ConstVertexVertexIter					CVVIter	;	//ConstVertexVertexIter type
	/*----------------------------------------------------------------------*/

	/*©w¸qÃB¥~¸ê®Æµ²ºc*/
	using namespace OpenMesh;
	//«ü©w¯S§Oµe¥X­±ªº¸ê®Æµ²ºc
	struct sp_f 
	{
		FHandle fh;
		float r, g, b;
	};
	//«ü©w¯S§Oµe¥X³»ÂIªº¸ê®Æµ²ºc
	struct sp_v 
	{
		VHandle vh;
		float r, g, b;
	};
	//«ü©w¥t¥~µe¥X¦ì¸mªº¸ê®Æµ²ºc
	struct sp_p
	{
		Point pt;
		float r, g, b;
	};
	/*----------------------------------------------------------------------*/
	struct srtPath
	{
		std::vector<OMP::VHandle> path;
	};

	/*©w¸qÃþ§O*/
	class Model:public MyMesh
	{
		//¸ê®Æ¦¨­û
	public:
		MyMesh Mesh;													//OpenMesh instance
	private:
		vector< sp_p > sp_p_list;
		vector< sp_v > sp_v_list;
		vector< sp_f > sp_f_list;
		//¨ç¦¡¦¨­û
	public:
		Model();//constructor
		~Model();//de-constructor

		bool ReadFile(std::string _fileName);//Åª¨úmesh¸ê®Æ
		bool SaveFile(std::string _fileName);//Àx¦smesh¸ê®Æ

		void Render_solid();			//solid rendering
		void Render_wireframe();		//wireframe rendering

		void RenderSpecifiedPoint();	//µe¥X«ü©w¦ì¸mªºÂI
		void RenderSpecifiedVertex();	//µe¥X«ü©wªº³»ÂI
		void RenderSpecifiedFace();		//µe¥X«ü©wªº­±

		void add_sp_p(Point   _p, float _r, float _g, float _b);//«ü©wÃB¥~µe¥XªºÂI
		void add_sp_v(VHandle _v, float _r, float _g, float _b);//«ü©wÃB¥~µe¥Xªº³»ÂI
		void add_sp_f(FHandle _f, float _r, float _g, float _b);//«ü©wÃB¥~µe¥Xªº­±
		void clear_sp_p();//²MªÅÃB¥~µe¥XªºÂI
		void clear_sp_v();//²MªÅÃB¥~µe¥Xªº³»ÂI
		void clear_sp_f();//²MªÅÃB¥~µe¥Xªº­±

		VHandle addVertex(Point _p);										//¦bmodel¤W¼W¥[·sªº³»ÂI
		FHandle addFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3);//¦bmodel¤W¼W¥[·sªº­±
		void deleteFace(FHandle _f);										//¦bmodel¤W§R°£­±
		void deleteFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3);//¦bmodel¤W§R°£­±

		bool IsVertexVertex( VHandle _vj, VHandle _vi);	//ÀË¬d¨â³»ÂI¬O§_¬Û¾F

		int quad_subdivision1(int _face_id);//1to4 quadrilateral subdivision
		int quad_subdivision2(int _face_id);//1to9 quadrilateral subdivision

	private:
		int findVertex(Point _p);//§ä¥X¦ì¸m_p¬O§_¤w¸g¦³³»ÂI¦s¦b
	};
}
/*======================================================================*/
class Tri_Mesh:public OMT::Model
{
public:
	Tri_Mesh()
	{
	
	
	}
	//-------Edit Flag-------//
    bool                                       Delete_Flag;
	
	int                                        Constrain_num;
	int                                        Boundary_num ;
	OMT::VHandle                               start_vh,end_vh;
	OMT::VHandle                               ExtremeVh[2];
	int                                        PatchType;

	std::vector<OMT::VHandle>                  Pluspt      ;
	std::vector<OMT::VHandle>                  Minuspt     ;
	std::vector<OMT::VHandle>                  Extrme_Pt   ;


	void Render_Solid();
	void Render_SolidWireframe();
	void Render_Wireframe();
	void Render_Point();

private:
};

///*======================================================================*/
/*======================================================================*/
bool ReadFile(std::string _fileName,Tri_Mesh *); //Åª¨úmesh¸ê®Æ
//	bool SaveFile(std::string _fileName);//Àx¦smesh¸ê®Æ
/*ªì©l¤Æview port³]©w¨ç¦¡*/

#endif

