// 2014-01-06, jjuiddong
//		- Render Composition Shape
// 
// 2016-12-22
//		- Refactoring
//
 #pragma once


#include "RenderBaseActor.h"
#include "foundation/PxVec3.h"

namespace SampleRenderer { class Renderer; class RendererShape; class RendererCompositionShape; }

class RenderComposition : public RenderBaseActor
{
public:
	RenderComposition(SampleRenderer::Renderer& renderer, 
		const int parentShapeIndex, const int childShapeIndex, 
		const int paletteIndex,
		const vector<PxTransform> &tmPalette,
		SampleRenderer::RendererCompositionShape *shape0, const PxTransform &tm0, 
		SampleRenderer::RendererCompositionShape *shape1, const PxTransform &tm1, 
		const PxReal* uvs=NULL);

	RenderComposition(SampleRenderer::Renderer& renderer, const int paletteIndex,
		const vector<PxTransform> &tmPalette, SampleRenderer::RendererShape *shape0, 
		const PxTransform &tm, RenderMaterial *material0);

	RenderComposition(const RenderComposition&);
	virtual ~RenderComposition();

	virtual void update(float deltaTime) override;
	virtual void render(SampleRenderer::Renderer& renderer, RenderMaterial* material=NULL, bool wireFrame = false) override;


protected:
	int m_PaletteIndex;
};
