#pragma once
#include "Core.h"
#include <string>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include "IInspectable.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;
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
	Quaternion Orientation;

	void LookAt(Vector3F Target);

	void AddRotation(Vector3F rotation);

	glm::mat4 GetRotationMatrix();

	Vector3F GetForwardVector();
	Vector3F GetRightVector();
	Vector3F GetUpVector();
	glm::mat4 GetTransformMatrix();

	glm::mat4 GetPointOfViewMatrix();


};

class FSceneObject : public IInspectable {

public:
	FSceneObject();
	FSceneObject(String name);
	~FSceneObject() {}
	virtual void Begin();
	virtual void End();
	virtual void Tick(float DeltaTime);
	virtual void PhysicsTick(float DeltaTime);
	virtual void Draw(glm::mat4 V, glm::mat4 P);
	virtual void OnDrawGUI(float Delta);
	virtual void Destroy();
	bool IsMarkedForDestroy() const { return MarkedForDestroy; }

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

	static SharedPtr<FSceneObject> Clone(FSceneObject& from);
protected:
	List<FObjectComponent*> Components;
	bool MarkedForDestroy = false;
	virtual void TickComponents(float Delta);

};


static void to_json(json& j, const FTransform& p) {
	json pos;
	json rot;
	json scale;
	to_json(pos, p.Position);
	to_json(rot, p.Rotation);
	to_json(scale, p.Scale);
	j = json{ {"Position", pos}, {"Rotation", rot}, {"Scale", scale} };
}

static void from_json(const json& j, FTransform& p) {
	from_json(j.at("Position"), p.Position);
	from_json(j.at("Rotation"), p.Rotation);
	from_json(j.at("Scale"), p.Scale);
}