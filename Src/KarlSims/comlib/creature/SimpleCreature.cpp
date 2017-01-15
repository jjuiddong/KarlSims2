
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

	return m_phRoot? true : false;
}


// Create Phenotype Node
phenotype::cNode* cSimpleCreature::CreatePhenotypeNode(
	phenotype::cNode *parent
	, const genotype::sBody &body
	, const genotype::sJoint *joint
	, const Vector3 &pos
	, const int depth)
	// joint = NULL, pos = Vector3(0, 0, 0), depth = 0
{
	if (depth > m_maxGenerationDepth)
		return CreateTerminalNode(parent, body);

	// Create RigidBody
	PxRigidDynamic *rigidBody = CreateBody(parent, body, joint, pos, depth);
	if (!rigidBody)
		return CreateTerminalNode(parent, body);

	phenotype::cNode *node = new phenotype::cNode();
	node->m_rigidBody = rigidBody;
	node->m_body = body;

	// Create Joint
	for (auto jointInfo : body.joints)
	{
		if (!jointInfo->link)
			continue;
		if (jointInfo->terminalOnly)
			continue; // If Terminal Only Node, Ignore

		phenotype::cNode *childNode = CreatePhenotypeNode(node, 
			*jointInfo->link, jointInfo, Vector3(0,0,0), depth+1);

		if (!childNode || (childNode == node))
			continue; // return if, not create or same instance to parent node (terminal node), already jointed

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


// Create Terminal Node
phenotype::cNode* cSimpleCreature::CreateTerminalNode(
	phenotype::cNode *parent, const genotype::sBody &body)
{
	RETV(!parent, NULL);

	// Create Terminal Joint
	for (auto jointInfo : body.joints)
	{
		if (!jointInfo->link)
			continue;
		if (!jointInfo->terminalOnly)
			continue; // Only Create Terminal Node

		phenotype::cNode *childNode = CreatePhenotypeNode(parent, *jointInfo->link, jointInfo);
		if (!childNode)
			continue;

		phenotype::cJoint *newJoint = new phenotype::cJoint();
		if (!newJoint->Init(parent, childNode, *jointInfo))
		{
			delete newJoint;
			break;
		}

		parent->m_joints.push_back(newJoint);
	}

	return parent;
}


PxRigidDynamic* cSimpleCreature::CreateBody( 
	phenotype::cNode *parent, const genotype::sBody &body 
	, const genotype::sJoint *joint
	, const Vector3 &pos, const int depth)
	// joint = NULL, pos = Vector3(0, 0, 0), depth = 0
{
	// Create RigidBody
	const float smallRate = pow(0.7f, depth);
	Vector3 dim;
	float radius = 0;
	switch (body.shape)
	{
	case eShapeType::BOX:
		dim = body.dim * smallRate;
		if (dim.Length() < 0.1f)
			return NULL; // too small body
		break;

	case eShapeType::SPHERE:
		radius = body.radius * smallRate;
		if (radius < 0.1f)
			return NULL; // too small body
		break;
	}

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
			curTm = PxTransform(PxQuat(joint->rot.x, 
				Vec3toPxVec3(Vector3(joint->rot.y, joint->rot.z, joint->rot.w)))) 
				* PxTransform(Vec3toPxVec3(joint->pos * smallRate));
		}
	}

	curTm = parentTm * curTm;
	if (!joint) // If Root Phenotype, Apply Root Position
		curTm = PxTransform(Vec3toPxVec3(pos)) * curTm;

	PxRigidDynamic *rigidBody = NULL;
	switch (body.shape)
	{
	case eShapeType::BOX:
		rigidBody = cPhysxMgr::Get()->CreateBox(
			curTm, Vec3toPxVec3(dim)
			, &PxVec3(0, 0, 0)
			, cPhysxMgr::Get()->GetMaterial(body.mtrl)
			, body.density);
		break;

	case eShapeType::SPHERE:
		rigidBody = cPhysxMgr::Get()->CreateSphere(
			curTm, radius
			, &PxVec3(0, 0, 0)
			, cPhysxMgr::Get()->GetMaterial(body.mtrl)
			, body.density);
		break;
	}	

	if (!rigidBody)
		return NULL; // error occur

	if (eBodyType::KINEMATIC == body.type)
	{
		rigidBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	}

	return rigidBody;
}
