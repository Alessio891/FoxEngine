#include "Graphics/RenderingPipeline.h"

void FRenderingPipeline::EnqueueRenderer(SharedPtr<FMeshRendererComponent> Renderer)
{
	auto found = std::find( EnqueuedRenderers.begin(), EnqueuedRenderers.end(), Renderer);
	if (found == EnqueuedRenderers.end()) {
		EnqueuedRenderers.push_back(Renderer);
	}
}

void FRenderingPipeline::PreRender(FTransform PointOfView, SharedPtr<FScene> Scene)
{
	if (Viewport == nullptr) return;

	glm::mat4 View = PointOfView.GetPointOfViewMatrix();
	glm::mat4 Projection = Viewport->GetProjectionMatrix();
	auto SceneObjects = Scene->GetSceneObjects();

	for (auto object : SceneObjects) {
		if (object->RenderingQueue == ERenderingQueue::PreRender)
		{
			object->Draw(View, Projection);
		}
	}
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FRenderingPipeline::Render(FTransform PointOfView, SharedPtr<FScene> Scene)
{
	if (Viewport == nullptr) return;

	glm::mat4 View = PointOfView.GetPointOfViewMatrix();
	glm::mat4 Projection = Viewport->GetProjectionMatrix();
	auto SceneObjects = Scene->GetSceneObjects();

	for (SharedPtr<FSceneObject> object : SceneObjects) {
		if (object->RenderingQueue == ERenderingQueue::Depth)
		{
			object->Draw(View, Projection);
		}
	}
	glClear(GL_DEPTH_BUFFER_BIT);
}

void FRenderingPipeline::PostRender(FTransform PointOfView, SharedPtr<FScene> Scene)
{
	if (Viewport == nullptr) return;

	glm::mat4 View = PointOfView.GetPointOfViewMatrix();
	glm::mat4 Projection = Viewport->GetProjectionMatrix();
	auto SceneObjects = Scene->GetSceneObjects();

	for (SharedPtr<FSceneObject> object : SceneObjects) {
		if (object->RenderingQueue == ERenderingQueue::Overlay)
		{
			object->Draw(View, Projection);
		}
	}
}
