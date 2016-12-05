#include "stdafx.h"
#include "SamplePreprocessor.h"
#include "JointTestSample.h"
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

#include "Creature/Creature.h"
#include "genetic/GeneticAlgorithm.h"
#include "diagram/GenotypeController.h"



using namespace SampleRenderer;
using namespace SampleFramework;

REGISTER_SAMPLE(CJointTestSample, "JointTest Sample")


///////////////////////////////////////////////////////////////////////////////

CJointTestSample::CJointTestSample(PhysXSampleApplication& app) :
	PhysXSample(app)
	, m_ElapsTime(NULL)
{
	mCreateGroundPlane = true;
	//m_IsApplyCustomGravity = true;
}

CJointTestSample::~CJointTestSample()
{
}

void CJointTestSample::onShutdown()
{
	BOOST_FOREACH(auto kv, m_Materials)
		kv.second->release();
	m_Materials.clear();

	PhysXSample::onShutdown();
}


void CJointTestSample::onInit()
{
	PhysXSample::onInit();

	CFileLoader::Get()->Init(NULL);

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
void CJointTestSample::customizeRender()
{
}


void	CJointTestSample::onTickPreRender(float dtime)
{
	PhysXSample::onTickPreRender(dtime);
	PxSceneWriteLock scopedLock(*mScene);
}

void	CJointTestSample::onTickPostRender(float dtime)
{
	PhysXSample::onTickPostRender(dtime);
}


void CJointTestSample::collectInputEvents(std::vector<const SampleFramework::InputEvent*>& inputEvents)
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
void CJointTestSample::spawnNode(const int key)
{
	PxSceneWriteLock scopedLock(*mScene);

	PxVec3 pos = getCamera().getPos() + (getCamera().getViewDir()*10.f);
	const PxVec3 vel = getCamera().getViewDir() * 20.f;

	evc::CCreature *creature = NULL;
	bool IsCreature = true;
	switch (key)
	{
	case SPAWN_DEBUG_OBJECT:
	{
		// spawn 2 rigid body
		const PxReal h = 4;
		const PxReal dim = 0.5f;
		PxRigidDynamic *actor0 = createBox(PxVec3(0, h, 0), PxVec3(dim, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(1, 1, 1)));
		PxRigidDynamic *actor1 = createBox(PxVec3(4, h, 0), PxVec3(dim, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(0, 0, 0)));
	}
	break;

	case SPAWN_DEBUG_OBJECT2:
	{
		// Fixed Joint Test
		// spawn 2 fixed joint rigid body
		const PxReal h = 4;
		const PxReal dim = 0.5f;
		const PxVec3 pos1(0, h, 0);
		const PxVec3 pos2(4, h, 2);
		PxRigidDynamic *actor0 = createBox(pos1, PxVec3(dim, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(1, 0, 0)));
		PxRigidDynamic *actor1 = createBox(pos2, PxVec3(dim, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(0, 1, 0)));

		// joint position is center of actor0,1
		PxVec3 v = pos2 - pos1;
		PxReal len =  v.magnitude();
		v.normalize();
		PxVec3 jpos = v*len + pos1;

		PxTransform tm0, tm1;
		tm0 = PxTransform(pos2 - jpos);
		tm1 = PxTransform(pos1 - jpos);
		PxFixedJoint *j = PxFixedJointCreate(getPhysics(), actor0, tm0, actor1, tm1);
	}
	break;

	case SPAWN_DEBUG_OBJECT3:
	{
		// Test RevoluteJoint
		const PxReal h = 4;
		const PxReal dim = 0.5f;
		const PxVec3 pos1(0, h, 0);
		const PxVec3 pos2(3, h, 0);
		PxRigidDynamic *actor0 = createBox(pos1, PxVec3(dim, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(1, 0, 0)));
		PxRigidDynamic *actor1 = createBox(pos2, PxVec3(dim*2, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(0, 1, 0)));
		actor0->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

		// joint position is center of actor0,1
		PxVec3 v = pos2 - pos1;
		PxReal len = v.magnitude();
		v.normalize();
		PxVec3 jpos = v*len + pos1;

		PxQuat q1 = rotationArc(PxVec3(1, 0, 0), PxVec3(0, 0, -1));

		PxTransform tm0, tm1;
		tm0 = PxTransform(pos2 - jpos);
		tm1 = PxTransform(pos1 - jpos) *  PxTransform(q1);
		PxFixedJoint *j = PxFixedJointCreate(getPhysics(), actor0, tm0, actor1, tm1);

		//if (PxRevoluteJoint*j = PxRevoluteJointCreate(getPhysics(), actor0, tm0, actor1, tm1))
		//{
		//	j->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);
		//}
	}
	break;

	case SPAWN_DEBUG_OBJECT4:
	{
		// Test Revolute Joint
		PxSceneWriteLock scopedLock(*mScene);

		const PxReal h = 4;
		const PxReal dim = 0.3f;
		const PxVec3 bpos1(0, h, 0);
		const PxVec3 bpos2(0, h, dim*2+0.1f);
		const PxVec3 pos1(0, h, 0);
		const PxVec3 pos2(3, h, 0);
		PxRigidDynamic *body0 = createBox(bpos1, PxVec3(dim*1.5f, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(0, 0, 0)));
		body0->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		PxRigidDynamic *body1 = createBox(bpos2, PxVec3(dim *1.5f, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(1, 1, 1)));
		body1->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

		PxRigidDynamic *arm1 = createBox(pos2, PxVec3(dim*2, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(0, 1, 0)));

		PxQuat q0 = rotationArc(PxVec3(1, 0, 0), PxVec3(1, 0, -1).getNormalized());
		PxQuat q1 = rotationArc(PxVec3(1, 0, 0), PxVec3(0, 0, -1));

		PxTransform tm0, tm1;
		tm0 = PxTransform(q0);
		tm1 = PxTransform(q1) * PxTransform(PxVec3(0, 0, -2));

		if (PxRevoluteJoint*j = PxRevoluteJointCreate(getPhysics(), body1, tm0, arm1, tm1))
		{
			j->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);
		}
	}
	break;

	case SPAWN_DEBUG_OBJECT5:
	{
		// Test Revolute Joint Force Test
		PxSceneWriteLock scopedLock(*mScene);

		const PxReal h = 4;
		const PxReal dim = 0.3f;
		const PxVec3 bpos1(0, h, 0);
		const PxVec3 bpos2(0, h, dim * 2 + 0.1f);
		const PxVec3 pos1(0, h, 0);
		const PxVec3 pos2(3, h, 0);
		PxRigidDynamic *body0 = createBox(bpos1, PxVec3(dim*1.5f, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(0, 0, 0)));
		body0->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		PxRigidDynamic *body1 = createBox(bpos2, PxVec3(dim *1.5f, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(1, 1, 1)));
		body1->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

		PxRigidDynamic *arm1 = createBox(pos2, PxVec3(dim * 2, dim, dim), &PxVec3(0, 0, 0), GetMaterial(PxVec3(0, 1, 0)));

		PxQuat q0 = rotationArc(PxVec3(1, 0, 0), PxVec3(1, 0, -1).getNormalized());
		PxQuat q1 = rotationArc(PxVec3(1, 0, 0), PxVec3(0, 0, -1));

		PxTransform tm0, tm1;
		tm0 = PxTransform(q0);
		tm1 = PxTransform(q1) * PxTransform(PxVec3(0, 0, -2));

		if (PxRevoluteJoint*j = PxRevoluteJointCreate(getPhysics(), body1, tm0, arm1, tm1))
		{
			j->setConstraintFlag(PxConstraintFlag::ePROJECTION, true);
		}
	}
	break;

	case SPAWN_DEBUG_OBJECT6:
	case SPAWN_DEBUG_OBJECT9:
	break;
	}

	RET(!creature);
}


/**
@brief
@date 2013-12-03
*/
void CJointTestSample::pickup()
{
	PxU32 width;
	PxU32 height;
	mApplication.getPlatform()->getWindowSize(width, height);
	mPicking->moveCursor(width / 2, height / 2);
	mPicking->lazyPick();
	PxActor *actor = mPicking->letGo();
	//PxRigidDynamic *rigidActor = static_cast<PxRigidDynamic*>(actor->is<PxRigidDynamic>());
	PxRigidDynamic *rigidActor = (PxRigidDynamic*)actor;
	if (rigidActor)
	{
		const PxVec3 pos = getCamera().getPos() + (getCamera().getViewDir()*10.f);
		const PxVec3 vel = getCamera().getViewDir() * 20.f;

		rigidActor->addForce(getCamera().getViewDir()*g_pDbgConfig->force);

		PxU32 nbShapes = rigidActor->getNbShapes();
		if (!nbShapes)
			return;

		PxShape** shapes = (PxShape**)SAMPLE_ALLOC(sizeof(PxShape*)*nbShapes);
		PxU32 nb = rigidActor->getShapes(shapes, nbShapes);
		PX_ASSERT(nb == nbShapes);
		for (PxU32 i = 0; i<nbShapes; i++)
		{
			RenderBaseActor *renderActor = getRenderActor(rigidActor, shapes[i]);
			if (renderActor)
			{
				renderActor->setRenderMaterial(mManagedMaterials[1]);
			}
		}
		SAMPLE_FREE(shapes);
	}
}


/**
@brief
@date 2013-12-03
*/
void CJointTestSample::onDigitalInputEvent(const SampleFramework::InputEvent &ie, bool val)
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
			pickup();
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
void CJointTestSample::onPointerInputEvent(const SampleFramework::InputEvent& ie,
	physx::PxU32 x, physx::PxU32 y, physx::PxReal dx, physx::PxReal dy, bool val)
{
	PhysXSample::onPointerInputEvent(ie, x, y, dx, dy, val);
}


void CJointTestSample::customizeSceneDesc(PxSceneDesc& sceneDesc)
{
	//sceneDesc.filterShader = SampleSubmarineFilterShader;
	//sceneDesc.simulationEventCallback = this;
	sceneDesc.flags |= PxSceneFlag::eREQUIRE_RW_LOCK;
}


/**
@brief
@date 2014-02-10
*/
void CJointTestSample::onSubstep(PxF32 dtime)
{
	m_ElapsTime += dtime;
	if (m_ElapsTime > 100) // 1 minutes
	{
		//gotoNextGenration();
		m_ElapsTime = 0;
	}
}


/**
@brief generate material
@date 2014-02-25
*/
RenderMaterial* CJointTestSample::GetMaterial(const PxVec3 &rgb, bool applyVertexColor) // applyVertexColor=true
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

