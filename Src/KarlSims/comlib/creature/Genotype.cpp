
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
		rotAxis = rhs.rotAxis;
		pos = rhs.pos;
		limit = rhs.limit;
		period = rhs.period;
		velocity = rhs.velocity;
		terminalOnly = rhs.terminalOnly;
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
		radius = rhs.radius;
		mtrl = rhs.mtrl;
		depth = rhs.depth;
		density = rhs.density;
		parent = rhs.parent;
		joints = rhs.joints;
	}
	return *this;
}
