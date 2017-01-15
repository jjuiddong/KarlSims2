//
// 2016-12-27, jjuiddong
// Tetrahedron (���ü) �� ǥ���ϴ� Ŭ����
//
#pragma once

#include "triangle3.h"

namespace delaunay3d
{

	class cTetrahedron
	{
	public:
		cTetrahedron();
		cTetrahedron(vector<Vector3> *vertices,
			const int idx0, const int idx1, const int idx2, const int idx3);

		void Create(vector<Vector3> *vertices,
			const int idx0, const int idx1, const int idx2, const int idx3);

		bool IsContain(const Vector3 &p);
		bool IsContainVertex(const Vector3 &p);

		bool operator==(const cTetrahedron &rhs) const;


	public:
		vector<Vector3> *m_vertices; // reference
		cTriangle m_tr[4];
		int m_indices[4];
		Vector3 m_circumCenter;
		float m_radius;
	};

}
