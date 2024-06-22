#include "Physics.h"
#include <Graphics/MaterialLibrary.h>
#include <glm/gtc/type_ptr.hpp>
void FPhysics::Initialize()
{
	if (!Initialized) {
		btBroadphase = SharedPtr<btDbvtBroadphase>(new btDbvtBroadphase());
		btCollisionConfiguration = SharedPtr<btDefaultCollisionConfiguration>(new btDefaultCollisionConfiguration());
		btDipatcher = SharedPtr<btCollisionDispatcher>(new btCollisionDispatcher(btCollisionConfiguration.get()));
		btSolver = SharedPtr<btSequentialImpulseConstraintSolver>(new btSequentialImpulseConstraintSolver());
		btWorld = SharedPtr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(btDipatcher.get(), btBroadphase.get(), btSolver.get(), btCollisionConfiguration.get()));
		btWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));
	}
	
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

	//collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -50, 0));

	btScalar mass(0.);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setRestitution(1.0f);

	//add the body to the dynamics world
	//btWorld->addRigidBody(body);
}

void FPhysics::StepSimulation(float Delta)
{
	btWorld->performDiscreteCollisionDetection();
	btWorld->stepSimulation(0.1f);
	
}

SharedPtr<btDbvtBroadphase> FPhysics::btBroadphase;
SharedPtr<btDefaultCollisionConfiguration> FPhysics::btCollisionConfiguration;
SharedPtr<btCollisionDispatcher> FPhysics::btDipatcher;
SharedPtr<btSequentialImpulseConstraintSolver> FPhysics::btSolver;
SharedPtr<btDiscreteDynamicsWorld> FPhysics::btWorld;
bool FPhysics::Initialized = false;

GLDebugDrawer::GLDebugDrawer()
	:m_debugMode(0)
{

}

void    GLDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(m_projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(m_viewMatrix));

	glColor3f(color.getX(), color.getY(), color.getZ());
	glLineWidth(0.1f);
	glBegin(GL_LINES);
	glVertex3f(from.getX(), from.getY(), from.getZ());
	glVertex3f(to.getX(), to.getY(), to.getZ());
	glEnd();
}

void    GLDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

void GLDebugDrawer::setMatrices(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	m_modelMatrix = modelMatrix;
	m_projectionMatrix = projectionMatrix;
	m_viewMatrix = viewMatrix;
}

void    GLDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	//glRasterPos3f(location.x(),  location.y(),  location.z());
	//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void    GLDebugDrawer::reportErrorWarning(const char* warningString)
{
	printf(warningString);
}

void    GLDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(m_projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(m_viewMatrix));

	glColor3f(color.getX(), color.getY(), color.getZ());
	glBegin(GL_POINTS);
	glVertex3f(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	glEnd();

	glBegin(GL_LINES);
	glLineWidth(0.1f);
	glVertex3f(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	glVertex3f(PointOnB.getX() + normalOnB.getX() * distance, PointOnB.getY() + normalOnB.getY() * distance, PointOnB.getZ() + normalOnB.getZ() * distance);
	glEnd();
}