//
// 2016-12-19, jjuiddong
// phenotype joint
//

#pragma once

#include "Genotype.h"


namespace evc {
	namespace phenotype {

		class cNode;

		class cJoint
		{
		public:
			cJoint();
			virtual ~cJoint();
			bool Init(phenotype::cNode *actor0, phenotype::cNode *actor1, const genotype::sJoint &joint);
			bool Update(const float dtime);
			void Clear();


		public:
			genotype::sJoint m_joint;
			phenotype::cNode *m_actor0; // parent body
			phenotype::cNode *m_actor1; // child body
			PxJoint *m_pxJoint;
			float m_incT; // for switch velocity
			//double m_relativeAngle; // radian, -pi ~ 0 ~ pi
			//PxTransform m_Tm0; // initial tm0
			//PxTransform m_Tm1; // initial tm1
		};

} }