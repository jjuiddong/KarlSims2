#pragma once


class cJoint
{
public:
	cJoint();
	cJoint(PxRigidDynamic *actor0, PxRigidDynamic *actor1, PxRevoluteJoint *joint);
	virtual ~cJoint();

	bool Init(PxRigidDynamic *actor0, PxRigidDynamic *actor1, PxRevoluteJoint *joint);
	void Update(float dtime);


protected:
		PxRigidDynamic *m_actor0;
		PxRigidDynamic *m_actor1;
		PxRevoluteJoint *m_joint;
		float m_dtime;
};
