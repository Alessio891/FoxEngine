#pragma once

#include "Core.h"
#include "MeshRendererComponent.h"
#include "Viewport.h"
#include <Application.h>

class FRenderingPipeline {
public:
	
	FRenderingPipeline(SharedPtr<FViewport> _Viewport) : Viewport(_Viewport) {};

	virtual void EnqueueRenderer(SharedPtr<FMeshRendererComponent> Renderer);

	virtual void PreRender(FTransform PointOfView, SharedPtr<FScene> Scene);
	virtual void Render(FTransform PointOfView, SharedPtr<FScene> Scene);
	virtual void PostRender(FTransform PointOfView, SharedPtr<FScene> Scene);

private:
	List<SharedPtr<FMeshRendererComponent>> EnqueuedRenderers;
	SharedPtr<FViewport> Viewport;
};
