#pragma once
#include "Core.h"
#include <string>
#include <glm/gtx/euler_angles.hpp>
#include "IInspectable.h"

class FObjectComponent;
class FMeshRendererComponent;

enum ERenderingQueue {
	PreRender = 0,
	Depth = 1,
	Overlay = 2
};


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

	glm::mat4 GetTransformMatrix() {
		glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), Position);
		ModelMatrix *= glm::eulerAngleXYX(Rotation.x, Rotation.y, Rotation.z);
		ModelMatrix = glm::scale(ModelMatrix, Scale);
		return ModelMatrix;
	}

	glm::mat4 GetPointOfViewMatrix() {
		Vector3F right = GetRightVector();
		Vector3F up = GetUpVector();

		glm::mat4 CameraMatrix = glm::lookAt(
			Position,
			Position + GetForwardVector(),
			up
		);

		return CameraMatrix;
	}
};

class FSceneObject : public IInspectable {

public:
	FSceneObject() : Name("[Object]"), IInspectable() {}
	FSceneObject(String name) : Name(name), IInspectable() {}
	~FSceneObject() {}
	virtual void Begin() {};
	virtual void End() {};
	virtual void Tick(float DeltaTime);

	virtual void Draw(glm::mat4 V, glm::mat4 P);

	virtual void AddComponent(FObjectComponent* Component);
	virtual void RemoveComponent(FObjectComponent* Component);
	virtual List<FObjectComponent*> const GetComponents() { return Components; }

	void SetupRenderer(FMeshRendererComponent* Renderer);

	virtual void DrawInspector() override;
	FTransform Transform;

	std::string Name = "[Object]";

	bool HideInHierarchy = false;

	ERenderingQueue RenderingQueue = ERenderingQueue::Depth;

	int ObjectID = -1;

	friend bool const operator==(const FSceneObject& lhs, const FSceneObject& rhs) {
		return lhs.ObjectID == rhs.ObjectID;
	}

	bool Outlined = false;

	SharedPtr<FMeshRendererComponent> Renderer;
protected:
	std::list<FObjectComponent*> Components;

	virtual void TickComponents(float Delta);

};