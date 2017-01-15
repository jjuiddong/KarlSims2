
#include "stdafx.h"
#include "Creature.h"
#include "PhenotypeNode.h"
#include "Joint.h"

using namespace evc;
using namespace genotype;
using namespace phenotype;


cCreature::cCreature()
	: m_phRoot(NULL)
	, m_maxGenerationDepth(2)
{
}

cCreature::~cCreature()
{
	Clear();
}


bool cCreature::Init(const string &genotypeFileName, const Vector3 &pos)
// pos=Vector3(0, 0, 0)
{
	Clear();

	RETV(!m_parser.Read(genotypeFileName), false);
	CreatePhenotype(pos);
	CreateSkinningMesh();

	return true;
}


bool cCreature::Update(const float dtime)
{
	if (m_phRoot)
		m_phRoot->Update(dtime);

	// Update Matrix Palette
	{
		PxSceneWriteLock scopedLock(*cPhysxMgr::Get()->m_sample->mScene);
		for each (auto &node in m_nodes)
		{
			if (node->m_rigidBody)
				m_tmPalette[node->m_PaletteIndex] = node->m_rigidBody->getGlobalPose();
		}
	}

	return true;
}


void cCreature::Clear()
{
	SAFE_DELETE(m_phRoot);
	m_nodes.clear();
}


bool cCreature::CreatePhenotype(const Vector3 &pos)
{
	RETV(!m_parser.m_root, false);

	Clear(); // remove, exist instance
	m_maxGenerationDepth = (m_parser.m_root->depth == 0) ? 2 : m_parser.m_root->depth;
	m_phRoot = CreatePhenotypeNode(NULL, *m_parser.m_root, NULL, pos);

	if (m_phRoot)
		m_phRoot->InitNeuralNet();

	return m_phRoot ? true : false;
}


// Create Phenotype Node
phenotype::cNode* cCreature::CreatePhenotypeNode(
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
	
	node->m_PaletteIndex = m_nodes.size();
	m_nodes.push_back(node);

	// Create Joint
	for (auto jointInfo : body.joints)
	{
		if (!jointInfo->link)
			continue;
		if (jointInfo->terminalOnly)
			continue; // If Terminal Only Node, Ignore

		phenotype::cNode *childNode = CreatePhenotypeNode(node,
			*jointInfo->link, jointInfo, Vector3(0, 0, 0), depth + 1);

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
phenotype::cNode* cCreature::CreateTerminalNode(
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


PxRigidDynamic* cCreature::CreateBody(
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
		parentTm = parent->m_rigidBody->getGlobalPose();

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


// generate skinning mesh from m_phRoot Node
void cCreature::CreateSkinningMesh()
{
	m_tmPalette.resize(m_nodes.size());
	CreateRenderComposition(m_phRoot);

	if (m_phRoot && m_phRoot->m_pOriginalShapeRenderer)
	{
		if (m_phRoot->m_pShapeRenderer)
		{
			cPhysxMgr::Get()->m_sample->addRenderObject(m_phRoot->m_pShapeRenderer);
		}
		else
		{
			m_phRoot->m_pShapeRenderer = m_phRoot->m_pOriginalShapeRenderer;
			cPhysxMgr::Get()->m_sample->addRenderObject(m_phRoot->m_pShapeRenderer);
		}
		m_tmPalette[m_phRoot->m_PaletteIndex] = m_phRoot->m_rigidBody->getGlobalPose();
	}
}


// composite all node shape
void cCreature::CreateRenderComposition(phenotype::cNode *node)
{
	RET(!node);

	for each (auto joint in node->m_joints)
		CreateRenderComposition(joint->m_actor1);

	PhysXSample *sample = cPhysxMgr::Get()->m_sample;
	RenderMaterial *material = cPhysxMgr::Get()->GetMaterial(node->m_body.mtrl);

	// Create RenderComposition
	if (!node->m_pOriginalShapeRenderer)
	{
		PxRigidDynamic *rigidActor0 = node->m_rigidBody;
		PxShape *shape0;
		if (1 == rigidActor0->getShapes(&shape0, 1))
		{
			RenderBaseActor *renderActor0 = sample->getRenderActor(rigidActor0, shape0);
			if (renderActor0)
			{
				RenderComposition *newRenderer = 
					SAMPLE_NEW(RenderComposition)(*sample->getRenderer(),
						node->m_PaletteIndex, m_tmPalette, renderActor0->getRenderShape(), material);

				renderActor0->setRendering(false);
				newRenderer->setEnableCameraCull(true);

				node->m_pOriginalShapeRenderer = newRenderer;
				sample->removeRenderObject(renderActor0);
				sample->unlink(renderActor0, shape0, rigidActor0);
			}
		}
	}

	// Remove Exist ShapeRenderer, If Diffrent
	if (node->m_pShapeRenderer && (node->m_pShapeRenderer != node->m_pOriginalShapeRenderer))
	{
		sample->removeRenderObject(node->m_pShapeRenderer);
		node->m_pShapeRenderer = NULL;
	}

	// Create Composition Child Shape
	for each (auto joint in node->m_joints)
	{
		cNode *child = (cNode*)joint->m_actor1;

		RenderComposition *parentRenderer = (node->m_pShapeRenderer) ? node->m_pShapeRenderer : node->m_pOriginalShapeRenderer;
		RenderComposition *childRenderer = (child->m_pShapeRenderer) ? child->m_pShapeRenderer : child->m_pOriginalShapeRenderer;

		RenderComposition *newRenderer = SAMPLE_NEW(RenderComposition)(*sample->getRenderer(),
			node->m_PaletteIndex, child->m_PaletteIndex,
			node->m_PaletteIndex, m_tmPalette,
			(SampleRenderer::RendererCompositionShape*)parentRenderer->getRenderShape(), joint->m_tm0,
			(SampleRenderer::RendererCompositionShape*)childRenderer->getRenderShape(), joint->m_tm1);

		newRenderer->setEnableCameraCull(true);

		// remove shape renderer
		if (node->m_pShapeRenderer && (node->m_pShapeRenderer != node->m_pOriginalShapeRenderer))
			sample->removeRenderObject(node->m_pShapeRenderer);

		node->m_pShapeRenderer = newRenderer;
	}
}
