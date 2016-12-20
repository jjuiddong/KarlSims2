
#include "stdafx.h"
#include "PhenotypeNode.h"
#include "Joint.h"

using namespace evc;
using namespace evc::phenotype;


cNode::cNode()
	: m_rigidBody(NULL)
{
}

cNode::~cNode()
{
	Clear();
}


bool cNode::Init(cNode *parent, const genotype::sBody &body, const Vector3 &pos)
// pos = Vector3(0,0,0)
{
	m_body = body;

	m_rigidBody = cPhysxMgr::Get()->CreateBox(
		PxTransform(Vec3toPxVec3(pos)), Vec3toPxVec3(body.dim)
		,&PxVec3(0,0,0)
		,cPhysxMgr::Get()->GetMaterial(body.mtrl)
		,body.mass);

	return true;
}


bool cNode::Update(const float dtime)
{
	for (auto j : m_joints)
		j->Update(dtime);

	return true;
}


void cNode::Clear() 
{
	m_rigidBody = NULL;

	for (auto j : m_joints)
		delete j;
	m_joints.clear();
}
