
#include "stdafx.h"
#include "utility.h"
#include "RendererProjection.h"

using namespace evc;


/**
@brief convertD3D9
@date 2014-02-24
*/
void evc::convertD3D9(D3DMATRIX &dxmat, const SampleRenderer::RendererProjection &mat)
{
	float temp[16];
	mat.getColumnMajor44(temp);
	for (PxU32 r = 0; r<4; r++)
	{
		for (PxU32 c = 0; c<4; c++)
		{
			dxmat.m[r][c] = temp[c * 4 + r];
		}
	}
}
