#pragma once
#include "Core.h"

class FObjectComponent;
class FMeshRendererComponent;

struct FTransform {
	Vector3F Position = Vector3F(0.0f,0.0f,0.0f);
	Vector3F Rotation = Vector3F(0.0f, 0.0f, 0.0f);
	Vector3F Scale = Vector3F(1.0f,1.0f,1.0f);

	Vector3F GetForwardVector() {
		return Vector3F(
			glm::cos(Rotation.x) * glm::sin(Rotation.y),
			glm::sin(Rotation.x),
			glm::cos(Rotation.x) * glm::cos(Rotation.y)
		);
	}

	Vector3F GetRightVector() {
		Vector3F right(
			glm::sin(Rotation.y - 3.14f / 2.0f),
			0,
			glm::cos(Rotation.x - 3.4f / 2.0f)
		);
		return right;
	}
	Vector3F GetUpVector() {
		Vector3F right = GetRightVector();
		Vector3F up = glm::cross(right, Vector3F(0,0,1));
		return up;
	}
};

class FSceneObject {

public:
	virtual void Begin() {};
	virtual void End() {};
	virtual void Tick(float DeltaTime);

	virtual void Draw(glm::mat4 V, glm::mat4 P);

	virtual void AddComponent(FObjectComponent* Component);
	virtual void RemoveComponent(FObjectComponent* Component);

	void SetupRenderer(FMeshRendererComponent* Renderer);

	virtual void DrawInspector();
	FTransform Transform;

	/*Vector3F Position;
	Vector3F Rotation;
	Vector3F Scale = Vector3F(1.0f,1.0f,1.0f);*/

protected:
	std::list<FObjectComponent*> Components;

	virtual void TickComponents(float Delta);

	class FMeshRendererComponent* Renderer;
};