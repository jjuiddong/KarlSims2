
#include "stdafx.h"
#include "joint.h"
#include "PhenotypeNode.h"


using namespace evc;
using namespace evc::phenotype;
using namespace genotype;


cJoint::cJoint()
: m_pxJoint(NULL)
, m_actor0(NULL)
, m_actor1(NULL)
, m_incT(0)
//, m_relativeAngle(0)
//, m_Tm0(tm0)
//, m_Tm1(tm1) 
{
}

cJoint::~cJoint()
{
	Clear();
}


bool cJoint::Init(phenotype::cNode *actor0, phenotype::cNode *actor1, 
	const genotype::sJoint &joint)
{
	m_joint = joint;
	m_actor0 = actor0;
	m_actor1 = actor1;

	// setting joint position, localFrame0, localFrame1
	const PxTransform actor0Tm = actor0->m_rigidBody->getGlobalPose();
	const PxTransform actor1Tm = actor1->m_rigidBody->getGlobalPose();
	const PxTransform jointTm((actor0Tm.p + actor1Tm.p) * 0.5f);
	const PxTransform tm0 = actor0Tm.getInverse() * jointTm;
	const PxTransform tm1 = actor1Tm.getInverse() * jointTm;

	switch (joint.type)
	{
	// Create Fixed Joint
	case eJointType::FIXED:
		if (PxFixedJoint *j = PxFixedJointCreate(
			cPhysxMgr::Get()->GetPhysics(),
			actor0->m_rigidBody, tm0, actor1->m_rigidBody, tm1))
		{
			j->setProjectionAngularTolerance(0);
			j->setBreakForce(40.0f, 40.0f);
			m_pxJoint = j;
		}
		break;

	// Create Revolute Joint
	case eJointType::REVOLUTE:
		if (PxRevoluteJoint *j = PxRevoluteJointCreate(
			cPhysxMgr::Get()->GetPhysics(),
			actor0->m_rigidBody, tm0, actor1->m_rigidBody, tm1))
		{
			j->setProjectionAngularTolerance(0);
			j->setProjectionLinearTolerance(0);
			
			if (joint.velocity != 0)
			{
				j->setDriveVelocity(joint.velocity);
				j->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
			}

			if (!joint.limit.IsEmpty())
			{
				// limit, upper, lower, tolerance
				j->setLimit(PxJointAngularLimitPair(joint.limit.x, joint.limit.y, joint.limit.z));
				j->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
			}

			j->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);

			m_pxJoint = j;
		}
		break;

		// Create Spherical Joint
	case eJointType::SPHERICAL:
		if (PxSphericalJoint *j = PxSphericalJointCreate(
			cPhysxMgr::Get()->GetPhysics(),
			actor0->m_rigidBody, tm0, actor1->m_rigidBody, tm1))
		{
			j->setProjectionLinearTolerance(0);

			if (!joint.limit.IsEmpty())
			{
				// cone limit, y, z, tolerance
				j->setLimitCone(PxJointLimitCone(joint.limit.x, joint.limit.y, joint.limit.z));
				j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
			}

			m_pxJoint = j;
		}
		break;
	}
	
	return true;
}


bool cJoint::Update(const float dtime)
{
	if (m_actor1)
		m_actor1->Update(dtime);

	if (eJointType::REVOLUTE == m_joint.type)
	{
		m_incT += dtime;
		if (m_incT > m_joint.period)
		{
			m_incT = 0;
			if (PxRevoluteJoint *j = (PxRevoluteJoint*)m_pxJoint)
				j->setDriveVelocity(j->getDriveVelocity() * -1.f);
		}
	}

	return true;
}


void cJoint::Clear()
{
	SAFE_RELEASE(m_pxJoint);
	SAFE_DELETE(m_actor1); // delete child Phenotype Node
}
