#pragma once
#include "Core.h"
#include "Scene.h"
#include <MeshRendererComponent.h>

class FSceneGizmo : public FSceneObject {
public:
	virtual void Begin() override;
	virtual void Draw(glm::mat4 V, glm::mat4 P) override;

protected:
	SharedPtr<MeshData> XAxis, YAxis, ZAxis;
	FMeshRendererComponent* YRenderer;
	FMeshRendererComponent* XRenderer;
	FMeshRendererComponent* ZRenderer;

	SharedPtr<FBaseMaterial> Material;
};
