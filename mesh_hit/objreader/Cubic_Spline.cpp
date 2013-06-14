
#include "Cubic_Spline.h"

namespace CSpline
{
	int Cubic_spline::setup_CSpline(vector< MyMesh::Vector3d > &_points)
	{
		if (_points.size() < 2)
		{
			std::cerr << "Cubic spline point number less than 2!\n";
			seg_list.clear();
			return 1;
		}

		using namespace LinearSystemLib;
		/*建立A矩陣*/
		GeneralSparseMatrix GA;
		GA.Create( _points.size(), _points.size());
		setup_matrixA(GA, _points.size());
		//建立B矩陣
		// only support double type.
		//-------------------------------------------------
		const int dim = 3;//x, y, z各一組
		double** B = new double*[dim]; // create 2d.

		B[0] = new double[_points.size()];
		B[1] = new double[_points.size()];
		B[2] = new double[_points.size()];

		for (unsigned int i = 0; i < _points.size(); ++i)
		{
			if (i == 0)
			{
				B[0][0] = 3*(_points[1][0] - _points[0][0]);
				B[1][0] = 3*(_points[1][1] - _points[0][1]);
				B[2][0] = 3*(_points[1][2] - _points[0][2]);
				continue;
			}
			if (i == _points.size()-1)
			{
				B[0][i] = 3*(_points[i][0] - _points[i-1][0]);
				B[1][i] = 3*(_points[i][1] - _points[i-1][1]);
				B[2][i] = 3*(_points[i][2] - _points[i-1][2]);
				continue;
			}
			B[0][i] = 3*(_points[i+1][0] - _points[i-1][0]);
			B[1][i] = 3*(_points[i+1][1] - _points[i-1][1]);
			B[2][i] = 3*(_points[i+1][2] - _points[i-1][2]);
		}

		// create the linear system A x = B.
		//-------------------------------------------------

		// note: 建立的 A and B memory 都將由 sls 負責管理，
		//	     ps. A and B must allocate from heap.
		// ps. GA 須轉成, stable sparse matrix.

		SparseLinearSystem sls( new StableSparseMatrix(GA), B, dim );

		// solving it !! A * x = B.
		//-------------------------------------------------

		try
		{

			// the solution will be stored here.
			double** S = 0;

			// solve it !!
			bool result = GeneralSparseLSSolver::GetInstance()->Solve( &sls, S );

			// output the solution..!!
			//-------------------------------------------------

			if( !result )
				cerr << "solve error!!! check your matrix." << endl;
			else
			{
				seg_list.clear();
				for (unsigned int i = 0; i < _points.size()-1; ++i)
				{
					Spline_ele spline_seg;
					spline_seg.ax = (double)( S[0][i+1] + S[0][i] - 2 * (_points[i+1][0] - _points[i][0]) );
					spline_seg.bx = (double)( 3 * (_points[i+1][0] - _points[i][0]) - 2 * S[0][i] - S[0][i+1] );
					spline_seg.cx = (double)( S[0][i] );
					spline_seg.dx = (double)( _points[i][0] );

					spline_seg.ay = (double)( S[1][i+1] + S[1][i] - 2 * (_points[i+1][1] - _points[i][1]) );
					spline_seg.by = (double)( 3 * (_points[i+1][1] - _points[i][1]) - 2 * S[1][i] - S[1][i+1] );
					spline_seg.cy = (double)( S[1][i] );
					spline_seg.dy = (double)( _points[i][1] );

					spline_seg.az = (double)( S[2][i+1] + S[2][i] - 2 * (_points[i+1][2] - _points[i][2]) );
					spline_seg.bz = (double)( 3 * (_points[i+1][2] - _points[i][2]) - 2 * S[2][i] - S[2][i+1] );
					spline_seg.cz = (double)( S[2][i] );
					spline_seg.dz = (double)( _points[i][2] );
					seg_list.push_back(spline_seg);
				}
			}

			// 須要自行 release solution memory, A，B 不用!!
			//-------------------------------------------------

			for( int i = 0 ; i < dim ; ++i )
				delete[] S[i];
			delete[] S;
			S = NULL;

		} 
		catch( exception e )
		{
			cerr << e.what() << endl;
		}
		return 0;
	}

	void Cubic_spline::setup_matrixA(LinearSystemLib::GeneralSparseMatrix& _matrix, int _size)
	{
		_matrix.SetElement( 0, 0, 2.0);
		for (int i = 1; i < _size-1; ++i)
		{
			_matrix.SetElement(i, i, 4.0);
		}
		for (int i = 0; i < _size-1; ++i)
		{
			_matrix.SetElement(i, i+1, 1.0);
			_matrix.SetElement(i+1, i, 1.0);
		}
		_matrix.SetElement( _size-1, _size-1, 2.0 );
	}
	/*----------------------------------------------------------------------*/
	MyMesh::Point Cubic_spline::get_position(int _seg, double _t)
	{
		MyMesh::Point position;
		position[0] = get_x(_seg, _t);
		position[1] = get_y(_seg, _t);
		position[2] = get_z(_seg, _t);
		return position;
	}

	MyMesh::Vector3d Cubic_spline::get_tangent(int _seg, double _t)
	{
		MyMesh::Vector3d tangent_dir;
		tangent_dir[0] = get_tangent_x(_seg, _t);
		tangent_dir[1] = get_tangent_y(_seg, _t);
		tangent_dir[2] = get_tangent_z(_seg, _t);
		return tangent_dir/tangent_dir.length();
	}
	/*----------------------------------------------------------------------*/
	double Cubic_spline::get_x(int _seg, double _t)
	{
		double t2 = _t*_t, t3 = t2*_t;
		return (double)( seg_list[_seg].ax * t3 + seg_list[_seg].bx * t2 + seg_list[_seg].cx * _t + seg_list[_seg].dx );
	}

	double Cubic_spline::get_y(int _seg, double _t)
	{
		double t2 = _t*_t, t3 = t2*_t;
		return (double)( seg_list[_seg].ay * t3 + seg_list[_seg].by * t2 + seg_list[_seg].cy * _t + seg_list[_seg].dy );
	}

	double Cubic_spline::get_z(int _seg, double _t)
	{
		double t2 = _t*_t, t3 = t2*_t;
		return (double)( seg_list[_seg].az * t3 + seg_list[_seg].bz * t2 + seg_list[_seg].cz * _t + seg_list[_seg].dz );
	}
	/*----------------------------------------------------------------------*/
	double Cubic_spline::get_tangent_x(int _seg, double _t)
	{
		double t2 = _t*_t;
		return (double)( 3 * seg_list[_seg].ax * t2 + 2 * seg_list[_seg].bx * _t + seg_list[_seg].cx );
	}

	double Cubic_spline::get_tangent_y(int _seg, double _t)
	{
		double t2 = _t*_t;
		return (double)( 3 * seg_list[_seg].ay * t2 + 2 * seg_list[_seg].by * _t + seg_list[_seg].cy );
	}

	double Cubic_spline::get_tangent_z(int _seg, double _t)
	{
		double t2 = _t*_t;
		return (double)( 3 * seg_list[_seg].az * t2 + 2 * seg_list[_seg].bz * _t + seg_list[_seg].cz );
	}
	/*----------------------------------------------------------------------*/
}