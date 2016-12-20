
#include "stdafx.h"
#include "SimpleCreature.h"
#include "PhenotypeNode.h"
#include "Joint.h"

using namespace evc;
using namespace genotype;


cSimpleCreature::cSimpleCreature()
: m_phRoot(NULL)
, m_maxGenerationDepth(2)
{
}

cSimpleCreature::~cSimpleCreature()
{
	Clear();
}


bool cSimpleCreature::Init(const string &genotypeFileName, const Vector3 &pos)
// pos=Vector3(0, 0, 0)
{
	Clear();

	RETV(!m_parser.Read(genotypeFileName), false);
	CreatePhenotype(pos);

	return true;
}


bool cSimpleCreature::Update(const float dtime)
{
	if (m_phRoot)
		m_phRoot->Update(dtime);
	return true;
}


void cSimpleCreature::Clear()
{
	SAFE_DELETE(m_phRoot);
}


bool cSimpleCreature::CreatePhenotype(const Vector3 &pos)
{
	RETV(!m_parser.m_root, false);

	m_maxGenerationDepth = (m_parser.m_root->depth==0)? 2 : m_parser.m_root->depth;
	m_phRoot = CreatePhenotypeNode(NULL, *m_parser.m_root, NULL, pos);

	return true;
}


phenotype::cNode* cSimpleCreature::CreatePhenotypeNode(
	phenotype::cNode *parent, const genotype::sBody &body
	, const genotype::sJoint *joint
	, const Vector3 &pos
	, const int depth)
	// joint = NULL, pos = Vector3(0, 0, 0), depth = 0
{
	RETV((depth > m_maxGenerationDepth), NULL);

	phenotype::cNode *node = new phenotype::cNode();

	node->m_body = body;

	// Create RigidBody
	//const float smallRate = pow(0.7f, depth);
	node->m_rigidBody = CreateBody(parent, body, joint, pos, depth);
	if (!node->m_rigidBody)
		return NULL;	

	// Create Child Joint
	for (auto jointInfo : body.joints)
	{
		if (!jointInfo->link)
			continue;

		phenotype::cNode *childNode = CreatePhenotypeNode(node, 
			*jointInfo->link, jointInfo, pos, depth+1);

		if (!childNode)
			continue;

		phenotype::cJoint *newJoint = new phenotype::cJoint();
		if (!newJoint->Init(node, childNode, *jointInfo))
		{
			delete newJoint;
			break;
		}

		node->m_joints.push_back(newJoint);
	}

	return node;
}


PxRigidDynamic* cSimpleCreature::CreateBody( 
	phenotype::cNode *parent, const genotype::sBody &body 
	, const genotype::sJoint *joint
	, const Vector3 &pos, const int depth)
	// joint = NULL, pos = Vector3(0, 0, 0), depth = 0
{
	// Create RigidBody
	const float smallRate = pow(0.7f, depth);
	const Vector3 dim = body.dim * smallRate;
	if (dim.Length() < 0.1f)
		return NULL; // too small body

	PxTransform parentTm = PxTransform::createIdentity();
	if (parent && parent->m_rigidBody)
		parentTm= parent->m_rigidBody->getGlobalPose();

	PxTransform curTm = PxTransform::createIdentity();
	if (joint)
	{
		if (joint->rot.IsEmpty())
		{
			curTm = PxTransform(Vec3toPxVec3(joint->pos * smallRate));
		}
		else
		{
			curTm = PxTransform(PxQuat(joint->rot.x, Vec3toPxVec3(Vector3(joint->rot.y, joint->rot.z, joint->rot.w)))) 
				* PxTransform(Vec3toPxVec3(joint->pos * smallRate));
		}
	}

	//PxTransform(Vec3toPxVec3(pos)) *
	curTm = parentTm * curTm;
	if (!joint)
		curTm = PxTransform(Vec3toPxVec3(pos)) * curTm;

	PxRigidDynamic *rigidBody = cPhysxMgr::Get()->CreateBox(
		curTm, Vec3toPxVec3(dim)
		, &PxVec3(0, 0, 0)
		, cPhysxMgr::Get()->GetMaterial(body.mtrl)
		, body.mass);

	if (!rigidBody)
		return NULL; // error occur

	if (eBodyType::KINEMATIC == body.type)
	{
		rigidBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	}

	return rigidBody;
}
