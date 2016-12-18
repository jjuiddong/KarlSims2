
#include "stdafx.h"
#include "joint.h"


cJoint::cJoint()
: m_actor0(NULL)
, m_actor1(NULL)
, m_joint(NULL)
, m_dtime(0)
{
}

cJoint::cJoint(PxRigidDynamic *actor0, PxRigidDynamic *actor1, PxRevoluteJoint *joint)
{
	Init(actor0, actor1, joint);
}

cJoint::~cJoint()
{
}


bool cJoint::Init(PxRigidDynamic *actor0, PxRigidDynamic *actor1, PxRevoluteJoint *joint)
{
	m_actor0 = actor0;
	m_actor1 = actor1;
	m_joint = joint;

	return true;
}


void cJoint::Update(float dtime)
{
	m_dtime += dtime;
	if (m_joint && (m_dtime>1))
	{
		m_dtime = 0;
		m_joint->setDriveVelocity(m_joint->getDriveVelocity() * -1.f);
	}
}
