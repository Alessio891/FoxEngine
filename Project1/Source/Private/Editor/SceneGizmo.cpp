#include "Editor/SceneGizmo.h"
#include <vector>
#include <Graphics/MaterialLibrary.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

void FSceneGizmo::Begin()
{
	FSceneObject::Begin();

	YAxis = SharedPtr<MeshData>(new MeshData());
	std::vector<float> vertices = {
			 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f
	};
	YAxis->VertexArray = std::list<float>(std::begin(vertices), std::end(vertices));
	YAxis->DrawType = GL_LINES;

	XAxis = SharedPtr<MeshData>(new MeshData());
	std::vector<float> xvertices = {
			 0.0f, 0.0f, 0.0f,
			 1.0f, 0.0f, 0.0f
	};
	XAxis->VertexArray = std::list<float>(std::begin(xvertices), std::end(xvertices));
	XAxis->DrawType = GL_LINES;

	ZAxis = SharedPtr<MeshData>(new MeshData());
	std::vector<float> zvertices = {
			 0.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 1.0f
	};
	ZAxis->VertexArray = std::list<float>(std::begin(zvertices), std::end(zvertices));
	ZAxis->DrawType = GL_LINES;

	YRenderer = new FMeshRendererComponent();
	YRenderer->MeshData = YAxis;
	XRenderer = new FMeshRendererComponent();
	XRenderer->MeshData = XAxis;
	ZRenderer = new FMeshRendererComponent();
	ZRenderer->MeshData = ZAxis;

	YRenderer->Color = Vector3F(0,1,0);
	XRenderer->Color = Vector3F(1,0,0);
	ZRenderer->Color = Vector3F(0,0,1);

	Material = SharedPtr<FBaseMaterial>(new FBaseMaterial(
		FMaterialLibrary::GetShader("Shaders/DefaultShader.vs", GL_VERTEX_SHADER), FMaterialLibrary::GetShader("Shaders/DefaultShader.fs", GL_FRAGMENT_SHADER), "DefaultMaterial2"
	));

	YRenderer->Material = XRenderer->Material = ZRenderer->Material = Material;

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
	YRenderer->Render(ModelMatrix, V, P);
	XRenderer->Render(ModelMatrix, V, P);
	ZRenderer->Render(ModelMatrix, V, P);
	
}
