// Examples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SphericalJointTest.h"
#include "stdafx.h"
#include "SamplePreprocessor.h"
#include "SampleUtils.h"
#include "SampleConsole.h"
#include "RendererMemoryMacros.h"
#include "RenderMeshActor.h"

#include "PxPhysics.h"
#include "PxScene.h"
#include "PxRigidDynamic.h"
#include "PxShape.h"
#include "PxPhysicsAPI.h"
#include "RenderBoxActor.h"

#include <SampleBaseInputEventIds.h>
#include <SamplePlatform.h>
#include <SampleUserInput.h>
#include <SampleUserInputIds.h>
#include <SampleUserInputDefines.h>



using namespace SampleRenderer;
using namespace SampleFramework;

REGISTER_SAMPLE(CSphericalJointTestSample, "SphericalJoint Test")


///////////////////////////////////////////////////////////////////////////////

CSphericalJointTestSample::CSphericalJointTestSample(PhysXSampleApplication& app) :
	PhysXSample(app)
	, m_ElapsTime(NULL)
	, m_genJoint(-1)
	, m_joint1(NULL)
	, m_joint2(NULL)
	, m_deltaTime(0)
{
	mCreateGroundPlane = true;
}

CSphericalJointTestSample::~CSphericalJointTestSample()
{
}

void CSphericalJointTestSample::onShutdown()
{
	for (auto kv : m_Materials)
		kv.second->release();
	m_Materials.clear();

	for (auto &j : m_joints)
		delete j;
	m_joints.clear();

	PhysXSample::onShutdown();
}


void CSphericalJointTestSample::onInit()
{
	PhysXSample::onInit();

	srand(timeGetTime());
	mApplication.setMouseCursorHiding(true);
	mApplication.setMouseCursorRecentering(true);
	mCameraController.init(PxVec3(0.0f, 3.0f, 10.0f), PxVec3(0.f, 0, 0.0f));
	mCameraController.setMouseSensitivity(0.5f);

	m_Planet.push_back(m_Ground);
}


/**
@brief
@date 2014-01-16
*/
void CSphericalJointTestSample::customizeRender()
{
}


void	CSphericalJointTestSample::onTickPreRender(float dtime)
{
	PhysXSample::onTickPreRender(dtime);
	PxSceneWriteLock scopedLock(*mScene);
}

void	CSphericalJointTestSample::onTickPostRender(float dtime)
{
	PhysXSample::onTickPostRender(dtime);
}


void CSphericalJointTestSample::collectInputEvents(std::vector<const SampleFramework::InputEvent*>& inputEvents)
{
	PhysXSample::collectInputEvents(inputEvents);
	getApplication().getPlatform()->getSampleUserInput()->unregisterInputEvent(CAMERA_SPEED_INCREASE);
	getApplication().getPlatform()->getSampleUserInput()->unregisterInputEvent(CAMERA_SPEED_DECREASE);

	//touch events
	DIGITAL_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT2, WKEY_2, XKEY_1, X1KEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT3, WKEY_3, XKEY_1, X1KEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT4, WKEY_4, XKEY_1, X1KEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT5, WKEY_5, XKEY_1, X1KEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT6, WKEY_6, XKEY_1, X1KEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT7, WKEY_7, XKEY_1, X1KEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT8, WKEY_8, XKEY_1, X1KEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT9, WKEY_9, XKEY_1, X1KEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT0, WKEY_0, XKEY_1, X1KEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(STEP_ONE_FRAME, WKEY_SPACE, XKEY_1, XKEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);

	DIGITAL_INPUT_EVENT_DEF(GOTO_NEXT_GENERATION, WKEY_BACKSPACE, XKEY_1, XKEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(REMOVE_OBJECT, WKEY_DELETE, XKEY_1, XKEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(GOTO_GENOTYPE_CONTROLLER, WKEY_HOME, XKEY_1, XKEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(GOTO_ORIENT_EDIT_MODE, WKEY_INSERT, XKEY_1, XKEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(SCALE_EDIT_MODE, WKEY_END, XKEY_1, XKEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(NEXT_OBJECT, WKEY_TAB, XKEY_1, XKEY_1, PS3KEY_1, PS4KEY_1, AKEY_UNKNOWN, OSXKEY_1, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, LINUXKEY_1, WIIUKEY_UNKNOWN);

	TOUCH_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT, "Throw Object", ABUTTON_5, IBUTTON_5, TOUCH_BUTTON_1);
	TOUCH_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT2, "Throw Object", ABUTTON_5, IBUTTON_5, TOUCH_BUTTON_1);
	TOUCH_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT3, "Throw Object", ABUTTON_5, IBUTTON_5, TOUCH_BUTTON_1);
	TOUCH_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT4, "Throw Object", ABUTTON_5, IBUTTON_5, TOUCH_BUTTON_1);
	TOUCH_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT5, "Throw Object", ABUTTON_5, IBUTTON_5, TOUCH_BUTTON_1);
	TOUCH_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT6, "Throw Object", ABUTTON_5, IBUTTON_5, TOUCH_BUTTON_1);
	TOUCH_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT7, "Throw Object", ABUTTON_5, IBUTTON_5, TOUCH_BUTTON_1);
	TOUCH_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT8, "Throw Object", ABUTTON_5, IBUTTON_5, TOUCH_BUTTON_1);
	TOUCH_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT9, "Throw Object", ABUTTON_5, IBUTTON_5, TOUCH_BUTTON_1);
	TOUCH_INPUT_EVENT_DEF(SPAWN_DEBUG_OBJECT0, "Throw Object", ABUTTON_5, IBUTTON_5, TOUCH_BUTTON_1);
}


/**
@brief
@date 2013-12-03
*/
void CSphericalJointTestSample::spawnNode(const int key)
{
	PxSceneWriteLock scopedLock(*mScene);

	PxVec3 pos = getCamera().getPos() + (getCamera().getViewDir()*10.f);
	const PxVec3 vel = getCamera().getViewDir() * 20.f;

	bool IsCreature = true;
	switch (key)
	{
	case SPAWN_DEBUG_OBJECT:
	{
		spawnDebugObject();
	}
	break;

	case SPAWN_DEBUG_OBJECT2:
	{
		// Spherical Joint Test
		PxSceneWriteLock scopedLock(*mScene);
		m_genJoint = 0;

		const PxReal h = 4;
		const PxReal dim = 0.3f;
		const PxReal force = 3;
		const PxVec3 bpos1(0, h, 0);
		const PxVec3 bpos2(0, h, -dim * 2 - 0.1f);

		int i = 0;
		const PxReal len = dim*1.5f;
		const PxVec3 pos0(i*(len + 0.1f) * 2, h, 0);
		PxTransform actor0Tm(pos0);
		PxRigidDynamic *actor0 = createBox2(actor0Tm, PxVec3(len, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(1, 1, 1)));
		actor0->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

		i++;
		const PxVec3 pos1(i*(len + 0.1f) * 2, h, 0);
		PxTransform actor1Tm(pos1);
		PxRigidDynamic *actor1 = createBox2(actor1Tm, PxVec3(len, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(1, 0, 0)));


		PxTransform jointTm((actor0Tm.p + actor1Tm.p) * 0.5f);
		PxTransform tm0 = actor0Tm.getInverse() * jointTm;
		PxTransform tm1 = actor1Tm.getInverse() * jointTm;
		if (PxSphericalJoint *j = PxSphericalJointCreate(getPhysics(), actor0, tm0, actor1, tm1))
		//if (PxDistanceJoint *j = PxDistanceJointCreate(getPhysics(), actor0, tm0, actor1, tm1))
		{
			j->setLimitCone(PxJointLimitCone(PxPi/4, PxPi/4));
			j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
		}
	}
	break;

	case SPAWN_DEBUG_OBJECT3:
	{
		// Test joint connection
		//   ===||====
		//   ===||====
		//
		PxSceneWriteLock scopedLock(*mScene);
		m_genJoint = 0;

		const PxReal h = 4;
		const PxReal dim = 0.3f;
		const PxReal force = 3;
		const PxVec3 bpos1(0, h, 0);
		const PxVec3 bpos2(0, h, -dim * 2 - 0.1f);
		PxRigidDynamic *parentBody = NULL;
		PxTransform parentTm;

		for (int i = 0; i < 5; ++i)
		{
			const PxReal len = dim*1.5f;
			const PxVec3 pos1(i*(len + 0.1f) * 2, h, 0);
			PxTransform actor0Tm(pos1);
			PxRigidDynamic *body0 = createBox2(actor0Tm, PxVec3(len, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(1, 1, 1)));

			const PxVec3 legPos1(i*(len + 0.1f) * 2, h, dim * 8 + 0.1f);
			PxTransform actor1Tm(legPos1, rotationArc(PxVec3(1, 0, 0), PxVec3(0, 0, -1)));
			PxRigidDynamic *leg0 = createBox2(actor1Tm, PxVec3(len, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(1, 0, 0)));

			{
				PxTransform jointTm(PxVec3(0, h, legPos1.z / 2));
				PxTransform tm0 = actor0Tm.getInverse() * jointTm;
				PxTransform tm1 = actor1Tm.getInverse() * jointTm;

				if (PxRevoluteJoint*j = PxRevoluteJointCreate(getPhysics(), body0, tm0, leg0, tm1))
				{
					j->setProjectionAngularTolerance(0);
					j->setProjectionLinearTolerance(0);
					j->setLimit(PxJointAngularLimitPair(-PxPi / 4, PxPi / 4, 0.1f)); // upper, lower, tolerance
					j->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
					j->setDriveVelocity(force);
					j->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
					j->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);

					m_joints.push_back(new cJoint(body0, leg0, j));
				}
			}

			const PxVec3 legPos2(i*(len + 0.1f) * 2, h, -(dim * 8 + 0.1f));
			PxTransform actor2Tm(legPos2, rotationArc(PxVec3(1, 0, 0), PxVec3(0, 0, -1)));
			PxRigidDynamic *leg1 = createBox2(actor2Tm, PxVec3(len, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(0, 1, 0)));

			{
				PxTransform jointTm(PxVec3(0, h, legPos2.z / 2));
				PxTransform tm0 = actor0Tm.getInverse() * jointTm;
				PxTransform tm1 = actor2Tm.getInverse() * jointTm;

				if (PxRevoluteJoint*j = PxRevoluteJointCreate(getPhysics(), body0, tm0, leg1, tm1))
				{
					j->setProjectionAngularTolerance(0);
					j->setProjectionLinearTolerance(0);
					j->setLimit(PxJointAngularLimitPair(-PxPi / 4, PxPi / 4, 0.1f)); // upper, lower, tolerance
					j->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
					j->setDriveVelocity(-force);
					j->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
					j->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);

					m_joints.push_back(new cJoint(body0, leg1, j));
				}
			}

			if (parentBody)
			{
				PxTransform jointTm((parentTm.p + actor0Tm.p) * 0.5f);
				PxTransform tm0 = parentTm.getInverse() * jointTm;
				PxTransform tm1 = actor0Tm.getInverse() * jointTm;
				//PxFixedJoint *j = PxFixedJointCreate(getPhysics(), parentBody, tm0, body0, tm1);
				//if (PxRevoluteJoint*j = PxRevoluteJointCreate(getPhysics(), parentBody, tm0, body0, tm1))
				if (PxSphericalJoint *j = PxSphericalJointCreate(getPhysics(), parentBody, tm0, body0, tm1))
				{
					//j->setLimitCone(PxJointLimitCone(PxPi / 4, PxPi / 4));
					//j->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
				}
			}

			parentBody = body0;
			parentTm = actor0Tm;
		}
	}
	break;

	case SPAWN_DEBUG_OBJECT9:
		break;
	}

}


/**
@brief
@date 2013-12-03
*/
void CSphericalJointTestSample::onDigitalInputEvent(const SampleFramework::InputEvent &ie, bool val)
{
	if (val)
	{
		switch (ie.m_Id)
		{
		case SPAWN_DEBUG_OBJECT:
		case SPAWN_DEBUG_OBJECT2:
		case SPAWN_DEBUG_OBJECT3:
		case SPAWN_DEBUG_OBJECT4:
		case SPAWN_DEBUG_OBJECT5:
		case SPAWN_DEBUG_OBJECT6:
		case SPAWN_DEBUG_OBJECT7:
		case SPAWN_DEBUG_OBJECT8:
		case SPAWN_DEBUG_OBJECT9:
		case SPAWN_DEBUG_OBJECT0:
			spawnNode(ie.m_Id);
			break;

		case PICKUP:
			break;

		case RELEASE_CURSOR:
		{ // nothing~
		}
		break;

		case GOTO_NEXT_GENERATION:
			break;

		default:
			PhysXSample::onDigitalInputEvent(ie, val);
			break;
		}
	}
}


/**
@brief pointer input event
@date 2014-02-12
*/
void CSphericalJointTestSample::onPointerInputEvent(const SampleFramework::InputEvent& ie,
	physx::PxU32 x, physx::PxU32 y, physx::PxReal dx, physx::PxReal dy, bool val)
{
	PhysXSample::onPointerInputEvent(ie, x, y, dx, dy, val);
}


void CSphericalJointTestSample::customizeSceneDesc(PxSceneDesc& sceneDesc)
{
	//sceneDesc.filterShader = SampleSubmarineFilterShader;
	//sceneDesc.simulationEventCallback = this;
	sceneDesc.flags |= PxSceneFlag::eREQUIRE_RW_LOCK;
}


/**
@brief
@date 2014-02-10
*/
void CSphericalJointTestSample::onSubstep(PxF32 dtime)
{
	m_deltaTime += dtime;
	m_ElapsTime += dtime;
	if (m_ElapsTime > 100) // 1 minutes
	{
		//gotoNextGenration();
		m_ElapsTime = 0;
	}

	if (0 == m_genJoint)
	{
		++m_genJoint;

		PxSceneWriteLock scopedLock(*mScene);
	}
	else if (1 == m_genJoint)
	{
		++m_genJoint;
		PxSceneWriteLock scopedLock(*mScene);
	}
	else if (2 == m_genJoint)
	{
		++m_genJoint;

		PxSceneWriteLock scopedLock(*mScene);
		//m_body0->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
		//m_leg0->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
		//m_body0->clearForce();
		//m_leg0->clearForce();
		//m_body0->wakeUp();
		//m_leg0->wakeUp();
		//m_genJoint = -1;
	}

	if (m_joint1 && (m_deltaTime>1))
	{
		m_joint1->setDriveVelocity(m_joint1->getDriveVelocity() * -1.f);
		m_joint2->setDriveVelocity(m_joint2->getDriveVelocity() * -1.f);
		m_deltaTime = 0;
	}

	for (auto j : m_joints)
		j->Update(dtime);

}


/**
@brief generate material
@date 2014-02-25
*/
RenderMaterial* CSphericalJointTestSample::GetMaterial(const PxVec3 &rgb, bool applyVertexColor) // applyVertexColor=true
{
	int key = (int)(rgb.x * 1000000 + rgb.y * 10000 + rgb.z * 100);
	key = key * 10 + applyVertexColor;

	auto it = m_Materials.find(key);
	if (m_Materials.end() != it)
		return it->second;

	const PxReal c = 0.75f;
	const PxReal opacity = 1.0f;
	const bool doubleSided = false;
	const PxU32 id = 0xffffffff;
	RenderMaterial *newMaterial = SAMPLE_NEW(RenderMaterial)(*getRenderer(), rgb, opacity,
		doubleSided, id, NULL, true, false, false, applyVertexColor);

	m_Materials[key] = newMaterial;
	return newMaterial;
}


PxRigidDynamic* CSphericalJointTestSample::createBox2(const PxTransform& tm, const PxVec3& dims, const PxVec3* linVel,
	RenderMaterial* material, PxReal density)
{
	PxSceneWriteLock scopedLock(*mScene);
	PxRigidDynamic* box = PxCreateDynamic(*mPhysics, tm, PxBoxGeometry(dims), *mMaterial, density);
	PX_ASSERT(box);

	box->setActorFlag(PxActorFlag::eVISUALIZATION, true);
	box->setAngularDamping(0.5f);
	box->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);

	mScene->addActor(*box);
	addPhysicsActors(box);

	if (linVel)
		box->setLinearVelocity(*linVel);

	createRenderObjectsFromActor(box, material);

	return box;
}
