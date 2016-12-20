//
// 2016-12-19, jjuiddong
// phenotype node
//
#pragma once

#include "Genotype.h"


namespace evc { namespace phenotype {

	class cJoint;
	class cNode
	{
	public:
		cNode();
		virtual ~cNode();
		bool Init(cNode *parent, const genotype::sBody &body, const Vector3 &pos=Vector3(0,0,0));
		bool Update(const float dtime);
		void Clear();


	public:
		genotype::sBody m_body;
		PxRigidDynamic *m_rigidBody;
		vector<cJoint*> m_joints;
	};

	} // phenotype
} // evc
