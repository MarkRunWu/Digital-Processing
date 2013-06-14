#ifndef _CH_H_
#define _CH_H_

#include "common.h"
#include <LinearSystemLib.h>

namespace CSpline
{
	//Ρ絬把计
	struct Spline_ele
	{
		double ax, bx, cx, dx,//X畒夹Ρ絬だ秖把计
			   ay, by, cy, dy,//Y畒夹Ρ絬だ秖把计
			   az, bz, cz, dz;//Z畒夹Ρ絬だ秖把计
	};

	class Cubic_spline
	{
	public:
		/*ミspline, 肚 -1:岿粇祇ネ, 0:タ盽挡, 1:翴计2*/
		int setup_CSpline(vector< MyMesh::Vector3d > &_points);
		/*眔琿计*/
		int n_segs(){return seg_list.size();};
		/*眔spline琘翴竚, seg琌琿, t琌把计*/
		MyMesh::Point get_position(int _seg, double _t);
		/*眔spline琘翴ち絬, seg琌琿, t琌把计*/
		MyMesh::Vector3d get_tangent(int _seg, double _t);
	protected:
	private:
		/**/
		void setup_matrixA(LinearSystemLib::GeneralSparseMatrix& _matrix, int _size);
		/*畒夹だ秖*/
		double get_x(int _seg, double _t);
		double get_y(int _seg, double _t);
		double get_z(int _seg, double _t);
		/*ち絬だ秖*/
		double get_tangent_x(int _seg, double _t);
		double get_tangent_y(int _seg, double _t);
		double get_tangent_z(int _seg, double _t);
		/*======================================================================*/
	public:
	protected:
	private:
		vector< Spline_ele > seg_list;
	};
}
#endif
