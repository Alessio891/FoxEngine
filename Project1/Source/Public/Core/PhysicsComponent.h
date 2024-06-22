#pragma once
#include "Core.h"
#include "ObjectComponent.h"
#include <Physics.h>

class FCollisionShape {
public:
	FCollisionShape();
	Vector3F Offset;

	virtual btCollisionShape* GetShape() {
		return nullptr;
	};

	virtual BString GetType() { return "None"; }
};

class FBoxShape : public FCollisionShape {
public:
	FBoxShape() : FCollisionShape() {}
	Vector3F Size = Vector3F(1,1,1);

	virtual btCollisionShape* GetShape() override;
	virtual BString GetType() { return "Box"; }
};

class FSphereShape : public FCollisionShape {
public:
	FSphereShape() : FCollisionShape() {}
	float Radius = 1.0f;
	virtual btCollisionShape* GetShape() override;
	virtual BString GetType() { return "Sphere"; }
};

class FPhysicsComponent : public FObjectComponent {

public:
	FPhysicsComponent();
	~FPhysicsComponent() {};
	virtual void Initialize(FSceneObject* Owner) override;
	virtual void Begin() override;
	virtual void End() override;
	virtual void Tick(float Delta) override;
	virtual void DrawInspector() override;
	virtual void ApplyPhysics();
	virtual void Render() override;
	virtual FObjectComponent* Clone() override;
	bool IsKinematic = true;
	SharedPtr<btRigidBody> Rigidbody;

	SharedPtr<FCollisionShape> Shape;

	float Mass = 10.0f;
	float Restitution = 0.0f;


protected:
	void SetShape(FCollisionShape* shape);
	void RegisterWithPhysics();

};