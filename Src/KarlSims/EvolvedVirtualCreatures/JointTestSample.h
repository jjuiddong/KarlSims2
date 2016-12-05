/**
*/
#pragma once

#include "PhysXSample.h"

//DECLARE_TYPE_NAME(CEvc)
class CJointTestSample : public PhysXSample
	, public PxSimulationEventCallback
	//, public memmonitor::Monitor<CEvc, TYPE_NAME(CEvc)>
{
public:
	CJointTestSample(PhysXSampleApplication& app);
	virtual ~CJointTestSample();

	// Implements PxSimulationEventCallback
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {}
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) {}
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor**, PxU32) {}
	virtual void onSleep(PxActor**, PxU32) {}


	//virtual	void onTickPreRender(float dtime) override;
	virtual	void customizeRender() override;
	virtual void	onTickPreRender(float dtime) override;
	virtual	void	onTickPostRender(float dtime) override;
	virtual	void customizeSceneDesc(PxSceneDesc&);

	//virtual	void	newMesh(const RAWMesh&);
	virtual	void	onInit();
	virtual	void	onSubstep(PxF32 dtime);

	virtual void collectInputEvents(std::vector<const SampleFramework::InputEvent*>& inputEvents);
	virtual void helpRender(PxU32 x, PxU32 y, PxU8 textAlpha) {}
	virtual	void descriptionRender(PxU32 x, PxU32 y, PxU8 textAlpha) {}
	virtual PxU32 getDebugObjectTypes() const;
	Picking* GetPicking();
	RenderMaterial* GetMaterial(const PxVec3 &rgb, bool applyVertexColor=true);


protected:
	void spawnNode(const int key);
	void pickup();
	virtual void onDigitalInputEvent(const SampleFramework::InputEvent&, bool val);
	virtual void onPointerInputEvent(const SampleFramework::InputEvent& ie, physx::PxU32 x, physx::PxU32 y, physx::PxReal dx, physx::PxReal dy, bool val) override;
	virtual void onShutdown();


private:
	vector<PxRigidActor*> m_Planet; // composite ground physx actor
	map<int, RenderMaterial*> m_Materials; // key=r*100,g*10,b, value = material

	double m_ElapsTime; // for gentic algorithm epoch
};


inline PxU32 CJointTestSample::getDebugObjectTypes() const {
	return DEBUG_OBJECT_BOX | DEBUG_OBJECT_SPHERE | DEBUG_OBJECT_CAPSULE | DEBUG_OBJECT_CONVEX;
}

inline Picking* CJointTestSample::GetPicking() { return mPicking; }