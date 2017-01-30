
#include "stdafx.h"
#include "viewer.h"
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

#include "creature/genotypeparser.h"


using namespace SampleRenderer;
using namespace SampleFramework;

REGISTER_SAMPLE(cGenotypeViewer, "Genotype Viewer")


///////////////////////////////////////////////////////////////////////////////

cGenotypeViewer::cGenotypeViewer(PhysXSampleApplication& app) :
	PhysXSample(app)
	, m_genJoint(-1)
	, m_joint1(NULL)
	, m_joint2(NULL)
	, m_deltaTime(0)
{
	mCreateGroundPlane = true;
}

cGenotypeViewer::~cGenotypeViewer()
{
}

void cGenotypeViewer::onShutdown()
{
	{
		PxSceneWriteLock scopedLock(*mScene);

		for (auto kv : m_Materials)
			kv.second->release();
		m_Materials.clear();

		for (auto &c : m_creatures)
			delete c;
		m_creatures.clear();
	}

	cPhysxMgr::Release();
	PhysXSample::onShutdown();
}


void cGenotypeViewer::onInit()
{
	PhysXSample::onInit();

	srand(timeGetTime());
	mApplication.setMouseCursorHiding(true);
	mApplication.setMouseCursorRecentering(true);
	mCameraController.init(PxVec3(0.0f, 3.0f, 10.0f), PxVec3(0.f, 0, 0.0f));
	mCameraController.setMouseSensitivity(0.5f);

	// Show Axis
	{
		PxSceneWriteLock scopedLock(*mScene);

		PxRigidDynamic *xAxis = createBox(PxVec3(10, 0, 0), PxVec3(10, 0.1f, 0.1f), NULL, 
			mManagedMaterials[MATERIAL_RED]);
		xAxis->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
		PxRigidDynamic *yAxis = createBox(PxVec3(0, 10, 0), PxVec3(0.1f, 10.f, 0.1f), NULL, 
			mManagedMaterials[MATERIAL_GREEN]);
		yAxis->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
		PxRigidDynamic *zAxis = createBox(PxVec3(0, 0, 10), PxVec3(0.1f, 0.1f, 10.f), NULL, 
			mManagedMaterials[MATERIAL_BLUE]);
		zAxis->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	}


	m_Planet.push_back(m_Ground);

	// initialize PhysX Manager instance
	cPhysxMgr::Get()->Init(this);
}


/**
@brief
@date 2014-01-16
*/
void cGenotypeViewer::customizeRender()
{
}


void	cGenotypeViewer::onTickPreRender(float dtime)
{
	PhysXSample::onTickPreRender(dtime);
	PxSceneWriteLock scopedLock(*mScene);
}

void	cGenotypeViewer::onTickPostRender(float dtime)
{
	PhysXSample::onTickPostRender(dtime);
}


void cGenotypeViewer::collectInputEvents(std::vector<const SampleFramework::InputEvent*>& inputEvents)
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
void cGenotypeViewer::spawnNode(const int key)
{
	PxSceneWriteLock scopedLock(*mScene);

	PxVec3 pos = getCamera().getPos() + (getCamera().getViewDir()*10.f);
	const PxVec3 vel = getCamera().getViewDir() * 20.f;

	bool IsCreature = true;
	switch (key)
	{
	case SPAWN_DEBUG_OBJECT:
		spawnDebugObject();
		break;
	
	case SPAWN_DEBUG_OBJECT2:
	{
		// Create Creatures
		evc::cCreature *creature = new evc::cCreature();
		creature->Init("../genotype_script/genotype_creature.txt", Vector3(0, 5, 0));
		m_creatures.push_back(creature);
	}
	break;

	case SPAWN_DEBUG_OBJECT3:
	{
		// Create Grass Creatures
		evc::cCreature *creature = new evc::cCreature();
		creature->Init("../genotype_script/genotype_grass.txt", Vector3(0, 0, 0));
		m_creatures.push_back(creature);
	}
	break;

	case SPAWN_DEBUG_OBJECT4:
	{
		// Create Tree Creatures
		evc::cCreature *creature = new evc::cCreature();
		creature->Init("../genotype_script/genotype_flower.txt", Vector3(0, 0, 0));
		m_creatures.push_back(creature);
	}
	break;

	case SPAWN_DEBUG_OBJECT5:
	{
		// Create Tree Creatures
		evc::cCreature *creature = new evc::cCreature();
		creature->Init("../genotype_script/genotype_tree.txt", Vector3(0, 0, 0));
		m_creatures.push_back(creature);
	}
	break;
	}
}


/**
@brief
@date 2013-12-03
*/
void cGenotypeViewer::onDigitalInputEvent(const SampleFramework::InputEvent &ie, bool val)
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
void cGenotypeViewer::onPointerInputEvent(const SampleFramework::InputEvent& ie,
	physx::PxU32 x, physx::PxU32 y, physx::PxReal dx, physx::PxReal dy, bool val)
{
	PhysXSample::onPointerInputEvent(ie, x, y, dx, dy, val);
}


void cGenotypeViewer::customizeSceneDesc(PxSceneDesc& sceneDesc)
{
	//sceneDesc.filterShader = SampleSubmarineFilterShader;
	//sceneDesc.simulationEventCallback = this;
	sceneDesc.flags |= PxSceneFlag::eREQUIRE_RW_LOCK;
}


/**
@brief
@date 2014-02-10
*/
void cGenotypeViewer::onSubstep(PxF32 dtime)
{
	m_deltaTime += dtime;	

	for (auto &c : m_creatures)
		c->Update(dtime);

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
}


/**
@brief generate material
@date 2014-02-25
*/
RenderMaterial* cGenotypeViewer::GetMaterial(const PxVec3 &rgb, bool applyVertexColor) // applyVertexColor=true
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


PxRigidDynamic* cGenotypeViewer::createBox2(const PxTransform& tm, const PxVec3& dims, const PxVec3* linVel,
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
