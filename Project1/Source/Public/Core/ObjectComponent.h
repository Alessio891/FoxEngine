#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class FSceneObject;

class FObjectComponent {

public:
	virtual void Initialize(FSceneObject* Owner) {
		this->Owner = Owner;
	};
	virtual void Begin() {};
	virtual void End() {};
	virtual void Tick(float DeltaTime) {};

	virtual void OnDrawGUI(float Delta)  {};

	virtual void DrawInspector();

	virtual void Deserialize(json json) {};

	virtual void MarkForRemoval() { MarkedForRemoval = true; }
	bool IsMarkedForRemoval() const { return MarkedForRemoval; }

protected:
	FSceneObject* Owner;
	bool MarkedForRemoval = false;
	bool CanTick = false;
};