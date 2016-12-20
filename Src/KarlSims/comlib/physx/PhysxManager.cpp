
#include "stdafx.h"
#include "PhysxManager.h"
#include "PhysXSample.h"

using namespace evc;


cPhysxMgr::cPhysxMgr()
	: m_sample(NULL)
{
}

cPhysxMgr::~cPhysxMgr()
{
	Clear();
}


bool cPhysxMgr::Init(PhysXSample *physxSample)
{
	m_sample = physxSample;

	return true;
}


void cPhysxMgr::Clear()
{
	for (auto kv : m_Materials)
		kv.second->release();
	m_Materials.clear();
}


PxRigidDynamic* cPhysxMgr::CreateBox(const PxTransform& tm, const PxVec3& dims, 
	const PxVec3* linVel, RenderMaterial* material, PxReal density)
{
	RETV(!m_sample, NULL);

	PxSceneWriteLock scopedLock(*m_sample->mScene);
	PxRigidDynamic* box = PxCreateDynamic(*m_sample->mPhysics, tm, 
		PxBoxGeometry(dims), *m_sample->mMaterial, density);
	PX_ASSERT(box);

	box->setActorFlag(PxActorFlag::eVISUALIZATION, true);
	box->setAngularDamping(0.5f);
	box->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);

	m_sample->mScene->addActor(*box);
	m_sample->addPhysicsActors(box);

	if (linVel)
		box->setLinearVelocity(*linVel);

	m_sample->createRenderObjectsFromActor(box, material);

	return box;
}


/**
@brief generate material
@date 2014-02-25
*/
RenderMaterial* cPhysxMgr::GetMaterial(const Vector3 &rgb, bool applyVertexColor) 
// applyVertexColor=true
{
	RETV(!m_sample, NULL);

	int key = (int)(rgb.x * 1000000 + rgb.y * 10000 + rgb.z * 100);
	key = key * 10 + applyVertexColor;

	auto it = m_Materials.find(key);
	if (m_Materials.end() != it)
		return it->second;

	const PxReal c = 0.75f;
	const PxReal opacity = 1.0f;
	const bool doubleSided = false;
	const PxU32 id = 0xffffffff;
	RenderMaterial *newMaterial = SAMPLE_NEW(RenderMaterial)(*m_sample->getRenderer(), 
		Vec3toPxVec3(rgb), opacity, doubleSided, id, NULL, true, false, false, applyVertexColor);

	m_Materials[key] = newMaterial;
	return newMaterial;
}


PxPhysics& cPhysxMgr::GetPhysics()
{ 
	return m_sample->getPhysics(); 
}
