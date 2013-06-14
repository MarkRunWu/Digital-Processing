#ifndef _CH_H_
#define _CH_H_

#include "common.h"
#include <LinearSystemLib.h>

namespace CSpline
{
	//���u�Ѽ�
	struct Spline_ele
	{
		double ax, bx, cx, dx,//X�y�Ц��u���q�Ѽ�
			   ay, by, cy, dy,//Y�y�Ц��u���q�Ѽ�
			   az, bz, cz, dz;//Z�y�Ц��u���q�Ѽ�
	};

	class Cubic_spline
	{
	public:
		/*�إ�spline, �^�ǭ� -1:���~�o��, 0:���`����, 1:�I�Ƥp��2*/
		int setup_CSpline(vector< MyMesh::Vector3d > &_points);
		/*���o�q��*/
		int n_segs(){return seg_list.size();};
		/*���ospline�W���Y�@�I��m, seg�O���@�q, t�O�Ѽ�*/
		MyMesh::Point get_position(int _seg, double _t);
		/*���ospline�W���Y�@�I���u, seg�O���@�q, t�O�Ѽ�*/
		MyMesh::Vector3d get_tangent(int _seg, double _t);
	protected:
	private:
		/**/
		void setup_matrixA(LinearSystemLib::GeneralSparseMatrix& _matrix, int _size);
		/*���y�Ф��q*/
		double get_x(int _seg, double _t);
		double get_y(int _seg, double _t);
		double get_z(int _seg, double _t);
		/*�����u���q*/
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
