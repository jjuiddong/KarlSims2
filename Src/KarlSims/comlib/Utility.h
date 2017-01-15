#pragma once


namespace SampleRenderer { class RendererProjection; }

namespace evc
{
	inline void quatRotationArc(PxQuat &q, const PxVec3& v0, const PxVec3& v1)
	{
		PxVec3 c = v0.cross(v1);
		if (c.isZero())
		{
			q = PxQuat(0, PxVec3(1, 0, 0));
			return;
		}

		float s = (float)sqrtf((1.0f + v0.dot(v1))*2.0f);
		q.x = c.x / s;
		q.y = c.y / s;
		q.z = c.z / s;
		q.w = s / 2.0f;
	}


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

	void convertD3D9(D3DMATRIX &dxmat, const SampleRenderer::RendererProjection &mat);

	// 보간.
	inline void lerp(OUT PxVec3 &out, const PxVec3 &a, const PxVec3 &b, float t)
	{
		out.x = a.x + (b.x - a.x) * t;
		out.y = a.y + (b.y - a.y) * t;
		out.z = a.z + (b.z - a.z) * t;
	}


	// 베지어 곡선을 리턴한다.
	// point[ 0 ~ 3]
	// t : 0 ~ 1
	inline void bezier(OUT PxVec3 &out, const vector<PxVec3> &points, const float t)
	{
		PxVec3 ab, bc, cd, abbc, bccd;
		lerp(ab, points[0], points[1], t);
		lerp(bc, points[1], points[2], t);
		lerp(cd, points[2], points[3], t);
		lerp(abbc, ab, bc, t);
		lerp(bccd, bc, cd, t);
		lerp(out, abbc, bccd, t);
	}

}
