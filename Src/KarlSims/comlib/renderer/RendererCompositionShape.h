//
// 2014-01-02, jjuiddong
//		- Composition Two Mesh
//
// 2016-12-22
//		- Refactoring
 //
#pragma once

#include <RendererShape.h>


class RenderMaterial;
namespace SampleRenderer
{
	class RendererVertexBuffer;
	class RendererIndexBuffer;

	class RendererCompositionShape : public RendererShape
	{
	public:
		RendererCompositionShape(Renderer &renderer, 
			const int parentShapeIndex, const int childShapeIndex, 
			const int paletteIndex, const vector<PxTransform> &tmPalette,
			RendererCompositionShape *shape0, const PxTransform &tm0, 
			RendererCompositionShape *shape1, const PxTransform &tm1, const PxReal* uvs=NULL);

		RendererCompositionShape(Renderer &renderer, const int paletteIndex, 
			const vector<PxTransform> &tmPalette, RendererShape *shape0, 
			const PxTransform &tm, RenderMaterial *material0 );

		virtual ~RendererCompositionShape(void);

		void ApplyPalette();


	protected:
		void GenerateCompositionShape( RendererShape *shape0, const PxTransform &tm0, 
			RendererShape *shape1, const PxTransform &tm1, 
			const int additionalVtxBuffSize, const int additionalIdxBuffSize );

		void GenerateCompositionShape( RendererShape *shape0, const PxTransform &tm, 
			RenderMaterial *material0 );

		void FindMostCloseFace(
			RendererShape *shape0, RendererShape *shape1,
			const int findParentBoneIndex, const int findChildBoneIndex,
			OUT set<PxU16> &vtxIndices0, OUT set<PxU16> &vtxIndices1,
			OUT vector<PxVec3> &vertices0, OUT vector<PxVec3> &vertices1);

		void CalculateCenterPoint( std::pair<int,int> closeFace0, std::pair<int,int> closeFace1,
			void *positions, PxU32 stride, PxU16 *indices, OUT PxVec3 &out );

		PxU32 GetColor(const void *colors, const void *bones, const PxU32 stride, 
			const int size, const PxU8 boneId);


	public:
		vector<PxVec3> m_SrcVertex;
		vector<PxVec3> m_SrcNormal;
		int m_PaletteIndex;
		const vector<PxTransform> &m_TmPalette;
		RendererVertexBuffer *m_vertexBuffer;
		RendererIndexBuffer  *m_indexBuffer;
	};
}
