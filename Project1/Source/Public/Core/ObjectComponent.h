#pragma once

class FSceneObject;

class FObjectComponent {

public:
	virtual void Initialize(FSceneObject* Owner) {
		this->Owner = Owner;
	};
	virtual void Tick(float DeltaTime) {};

	virtual void DrawInspector();

protected:
	FSceneObject* Owner;
};