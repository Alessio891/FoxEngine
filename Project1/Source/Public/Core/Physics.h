#pragma once

#include "Core.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "SceneObject.h"
#include <PhysicsComponent.h>

#ifndef GL_DEBUG_DRAWER_H
#define GL_DEBUG_DRAWER_H

class GLDebugDrawer : public btIDebugDraw
{
	int m_debugMode;

public:

	GLDebugDrawer();

	virtual void   drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	virtual void   drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	virtual void   reportErrorWarning(const char* warningString);

	virtual void   draw3dText(const btVector3& location, const char* textString);

	virtual void   setDebugMode(int debugMode);

	virtual int      getDebugMode() const { return m_debugMode; }
	void setMatrices(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
private:
	glm::mat4 m_modelMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
};
#endif

class FPhysics {
public:
	static void Initialize();
	static void StepSimulation(float Delta);


	//static btRigidBody* RegisterObject(FSceneObject* object, bool kinematic = false);
	static SharedPtr<btDiscreteDynamicsWorld> btWorld;
protected:
	static SharedPtr<btDbvtBroadphase> btBroadphase;
	static SharedPtr<btDefaultCollisionConfiguration> btCollisionConfiguration;
	static SharedPtr<btCollisionDispatcher> btDipatcher;
	static SharedPtr<btSequentialImpulseConstraintSolver> btSolver;

	static bool Initialized;


};
