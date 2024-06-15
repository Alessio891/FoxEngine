#include "MeshRendererComponent.h"
#include "Graphics/Graphics.h"

bool FGraphics::Raycast(
	Vector3F Origin, 
	Vector3F Direction, 
	FSceneObject* Target,
	float& Distance
	)
{
	float tMin = 0.0f;
	float tMax = 100000.0f;
	FTransform TargetTransform = Target->Transform;
	FBoundingBox BB = Target->Renderer->MeshDataRef->BoundingBox;

	glm::mat4 ModelMatrix = TargetTransform.GetTransformMatrix();
	
	Vector3F delta = TargetTransform.Position - Origin;
	{
		Vector3F xAxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xAxis, delta);
		float f = glm::dot(Direction, xAxis);
		float t1 = (e + BB.Min.x) / f;
		float t2 = (e + BB.Max.x) / f;

		if (t1 > t2) {
			float w=t1; t1=t2; t2=w;
		}

		if (t2 < tMax) tMax = t2;
		if (t1 > tMin) tMin = t1;

		if (tMax < tMin) return false;
	}
	{
		Vector3F yAxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = glm::dot(yAxis, delta);
		float f = glm::dot(Direction, yAxis);

		float t1 = (e + BB.Min.x) / f;
		float t2 = (e + BB.Max.x) / f;

		if (t1 > t2) {
			float w = t1; t1 = t2; t2 = w;
		}

		if (t2 < tMax) tMax = t2;
		if (t1 > tMin) tMin = t1;

		if (tMax < tMin) return false;
	}

	{
		Vector3F zAxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
		float e = glm::dot(zAxis, delta);
		float f = glm::dot(Direction, zAxis);

		float t1 = (e + BB.Min.x) / f;
		float t2 = (e + BB.Max.x) / f;

		if (t1 > t2) {
			float w = t1; t1 = t2; t2 = w;
		}

		if (t2 < tMax) tMax = t2;
		if (t1 > tMin) tMin = t1;

		if (tMax < tMin) return false;
	}
	return true;
}
