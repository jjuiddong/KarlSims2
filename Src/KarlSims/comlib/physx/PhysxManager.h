//
// 2016-12-19, jjuiddong
// PhysX Sample Manager
//
#pragma once


class PhysXSample;

class cPhysxMgr : public common::cSingleton<cPhysxMgr>
{
public:
	cPhysxMgr();
	virtual ~cPhysxMgr();
	bool Init(PhysXSample *physxSample);
	void Clear();

	PxRigidDynamic* CreateBox(const PxTransform &tm, const PxVec3& dims, const PxVec3* linVel = NULL,
		RenderMaterial* material = NULL, PxReal density = 1.0f);

	RenderMaterial* GetMaterial(const Vector3 &rgb, bool applyVertexColor=true);

	PxPhysics& GetPhysics();


protected:
	PhysXSample *m_sample;
	map<int, RenderMaterial*> m_Materials; // key=r*100,g*10,b, value = material
};
