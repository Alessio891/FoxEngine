#include "Editor/SceneGizmo.h"
#include <vector>
#include <Graphics/MaterialLibrary.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

void FSceneGizmo::Begin()
{
	FSceneObject::Begin();

	YAxis = SharedPtr<MeshData>(new MeshData({
			 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f
		}));
	YAxis->DrawType = GL_LINES;

	XAxis = SharedPtr<MeshData>(new MeshData({
			 0.0f, 0.0f, 0.0f,
			 1.0f, 0.0f, 0.0f
		}));
	XAxis->DrawType = GL_LINES;

	ZAxis = SharedPtr<MeshData>(new MeshData({
			 0.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 1.0f
		}));
	ZAxis->DrawType = GL_LINES;

	YRenderer = new FMeshRendererComponent();
	YRenderer->MeshDataRef = YAxis;
	XRenderer = new FMeshRendererComponent();
	XRenderer->MeshDataRef = XAxis;
	ZRenderer = new FMeshRendererComponent();
	ZRenderer->MeshDataRef = ZAxis;

	YRenderer->UseMeshAsset = XRenderer->UseMeshAsset = ZRenderer->UseMeshAsset = false;
	YRenderer->Color = Vector3F(0,1,0);
	XRenderer->Color = Vector3F(1,0,0);
	ZRenderer->Color = Vector3F(0,0,1);

	Material = FMaterialLibrary::GetMaterial("DefaultUnlit");

	YRenderer->Material.Set(Material);
	XRenderer->Material.Set(Material);
	ZRenderer->Material.Set(Material);

	YRenderer->Initialize(this);
	XRenderer->Initialize(this);
	ZRenderer->Initialize(this);

	RenderingQueue = ERenderingQueue::Overlay;
}

void FSceneGizmo::Draw(glm::mat4 V, glm::mat4 P)
{
	FSceneObject::Draw(V, P);
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), Transform.Position);
	ModelMatrix *= glm::eulerAngleXYX(Transform.Rotation.x, Transform.Rotation.y, Transform.Rotation.z);
	ModelMatrix = glm::scale(ModelMatrix, Transform.Scale);
	glDisable(GL_CULL_FACE);
	YRenderer->Render(V, P);
	XRenderer->Render(V, P);
	ZRenderer->Render(V, P);
	
}
