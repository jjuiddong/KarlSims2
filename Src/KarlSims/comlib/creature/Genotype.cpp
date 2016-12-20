
#include "stdafx.h"
#include "Genotype.h"

using namespace genotype;

sJoint::sJoint()
{
}

sJoint::sJoint(const sJoint &rhs)
{
	operator=(rhs);
}

sJoint& sJoint::operator=(const sJoint &rhs)
{
	if (this != &rhs)
	{
		type = rhs.type;
		rot = rhs.rot;
		pos = rhs.pos;
		limit = rhs.limit;
		period = rhs.period;
		velocity = rhs.velocity;
		linkName = rhs.linkName;
		link = rhs.link;
	}
	return *this;
}



sBody::sBody()
{
}

sBody::sBody(const sBody &rhs)
{
	operator=(rhs);
}

sBody& sBody::operator=(const sBody &rhs)
{
	if (this != &rhs)
	{
		name = rhs.name;
		type = rhs.type;
		shape = rhs.shape;
		dim = rhs.dim;
		mtrl = rhs.mtrl;
		depth = rhs.depth;
		mass = rhs.mass;
		parent = rhs.parent;
		joints = rhs.joints;
	}
	return *this;
}
