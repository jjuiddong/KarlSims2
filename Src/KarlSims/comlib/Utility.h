#pragma once


namespace SampleRenderer { class RendererProjection; }

namespace evc
{
	/**
	@brief Vector3 -> PxVec3
	@date 2014-02-24
	*/
	inline const PxVec3 Vec3toPxVec3(const Vector3 &v)
	{
		return PxVec3(v.x, v.y, v.z);
	}

	/**
	@brief PxVec3 -> Vector3
	@date 2014-02-24
	*/
	inline const Vector3 PxVec3toVec3(const  PxVec3 &v)
	{
		return Vector3(v.x, v.y, v.z);
	}


	/**
	@brief convertD3D9
	@date 2014-02-24
	*/
	inline void convertD3D9(D3DMATRIX &dxmat, const physx::PxMat44 &mat)
	{
		memcpy(&dxmat._11, mat.front(), 4 * 4 * sizeof(float));
	}


	//void convertD3D9(D3DMATRIX &dxmat, const SampleRenderer::RendererProjection &mat);
	//void lerp(OUT PxVec3 &out, const PxVec3 &a, const PxVec3 &b, float t);
	//void bezier(OUT PxVec3 &out, const vector<PxVec3> &points, const float t);

}
