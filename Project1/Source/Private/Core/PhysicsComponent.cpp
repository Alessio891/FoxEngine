#include "PhysicsComponent.h"
#include "GUI\GUI.h"
#include "Scene.h"

FPhysicsComponent::FPhysicsComponent()
{
	if (Shape == nullptr) {
		SharedPtr<FBoxShape> shape(new FBoxShape());
		shape->Size = Vector3F(1, 1, 1);
		Shape = std::static_pointer_cast<FCollisionShape>(shape);
	}
	
}

void FPhysicsComponent::Initialize(FSceneObject* Owner)
{
	FObjectComponent::Initialize(Owner);
	RegisterWithPhysics();
	auto quatRot = Owner->Transform.Orientation;
	btQuaternion rot(quatRot.x, quatRot.y, quatRot.z, quatRot.w);

	btTransform t(
		rot
	);
	t.setOrigin(btVector3(Owner->Transform.Position.x, Owner->Transform.Position.y, Owner->Transform.Position.z));
	Rigidbody->setWorldTransform(t);
	Rigidbody->getCollisionShape()->setLocalScaling(btVector3(Owner->Transform.Scale.x, Owner->Transform.Scale.y, Owner->Transform.Scale.z));
	if (!FApplication::Get()->GetCurrentScene()->IsPlaying()) {
		Rigidbody->activate(false);
	}
}

void FPhysicsComponent::Begin()
{
	auto quatRot = Owner->Transform.Orientation;
	btQuaternion rot(quatRot.x, quatRot.y, quatRot.z, quatRot.w);

	btTransform t(
		rot
	);
	t.setOrigin(btVector3(Owner->Transform.Position.x, Owner->Transform.Position.y, Owner->Transform.Position.z));
	Rigidbody->setWorldTransform(t);
	Rigidbody->getCollisionShape()->setLocalScaling(btVector3(Owner->Transform.Scale.x, Owner->Transform.Scale.y, Owner->Transform.Scale.z));
	if (!FApplication::Get()->GetCurrentScene()->IsPlaying()) {
		Rigidbody->activate(false);
	}
	Rigidbody->setLinearVelocity(btVector3(0,0,0));
}

void FPhysicsComponent::End()
{
	if (Rigidbody != nullptr) {
	/*	FPhysics::btWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(Rigidbody->getBroadphaseHandle(), FPhysics::btWorld->getDispatcher());
		FPhysics::btWorld->removeRigidBody(Rigidbody.get());
		Rigidbody.reset();*/
	}
}

void FPhysicsComponent::Tick(float Delta)
{
	if (Rigidbody != nullptr) {
		
		auto quatRot = Owner->Transform.Orientation;
		btQuaternion rot(quatRot.x, quatRot.y, quatRot.z, quatRot.w);
		
		btTransform t(
			rot
		);
		t.setOrigin(btVector3(Owner->Transform.Position.x, Owner->Transform.Position.y, Owner->Transform.Position.z));
		Rigidbody->setWorldTransform(t);
		Rigidbody->getCollisionShape()->setLocalScaling(btVector3(Owner->Transform.Scale.x, Owner->Transform.Scale.y, Owner->Transform.Scale.z));
		Rigidbody->setRestitution(Restitution);
		if (!IsKinematic) {
			Rigidbody->activate(true);
			Rigidbody->setGravity(btVector3(0.0f, -9.8f, 0.0f));
		}
		else {
			Rigidbody->setGravity(btVector3(0.0f, 0.0f, 0.0f));
		}
	}
}

void FPhysicsComponent::DrawInspector()
{
	
	if (Rigidbody != nullptr) {
		ImGui::InputFloat("Mass", &Mass, 0.1f);
		ImGui::InputFloat("Restitution", &Restitution, 0.1f);
		float r = Rigidbody->getRestitution();
		char buf[100];
		sprintf(buf, "R: %.2f", r);
		ImGui::Text(buf);

		btVector3 inertia(0,0,0);
		if (Rigidbody->getCollisionShape() != nullptr) {
			Rigidbody->getCollisionShape()->calculateLocalInertia(Mass, inertia);
		}
		Rigidbody->setMassProps(Mass, inertia);
		ImGui::Checkbox( "IsKinematic", &IsKinematic );
		if (ImGui::BeginCombo("Shape", Shape->GetType().c_str())) {
			if (ImGui::Selectable("Box", false)) {
				if (Shape->GetType() != "Box") {
					SetShape(new FBoxShape());
				}
			}
			if (ImGui::Selectable("Sphere", false)) {
				if (Shape->GetType() != "Sphere") {
					SetShape(new FSphereShape());
				}
			}
			ImGui::EndCombo();
		}
		BString type = Shape->GetType();
		if (type == "Box") {
			SharedPtr<FBoxShape> box = std::static_pointer_cast<FBoxShape>(Shape);
			box->Size = FGUI::Vec3("Size", box->Size);
		}
		else if (type == "Sphere") {
			SharedPtr<FSphereShape> box = std::static_pointer_cast<FSphereShape>(Shape);
			ImGui::DragFloat("Radius", &box->Radius);
			btSphereShape* sphere = dynamic_cast<btSphereShape*>(Rigidbody->getCollisionShape());
			sphere->setUnscaledRadius(box->Radius);
		}
	}	
	
}

void FPhysicsComponent::ApplyPhysics()
{
	if (!IsKinematic && Rigidbody != nullptr) {
		auto pos = Rigidbody->getWorldTransform().getOrigin();
		auto rot = Rigidbody->getWorldTransform().getRotation();
		Owner->Transform.Orientation = glm::quat(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
		Owner->Transform.Position = Vector3F(pos.getX(), pos.getY(), pos.getZ());
	}
}

void FPhysicsComponent::Render()
{
	if (Rigidbody == nullptr || Rigidbody->getCollisionShape() == nullptr) return;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(0);

	auto proj = FApplication::Get()->GameViewport->GetProjectionMatrix();
	auto view = FApplication::Get()->GetCurrentScene()->CameraTransform.GetPointOfViewMatrix();
	auto model = Owner->Transform.GetTransformMatrix();
	auto drawer = static_cast<GLDebugDrawer*>(FPhysics::btWorld->getDebugDrawer());
	drawer->setMatrices(model, view, proj);

	BString type = Shape->GetType();
	if (type == "Box") {
		SharedPtr<FBoxShape> box = std::static_pointer_cast<FBoxShape>(Shape);
		btBoxShape* b = static_cast<btBoxShape*>(Rigidbody->getCollisionShape());
		btVector3 m = b->getHalfExtentsWithMargin();
		
		FPhysics::btWorld->getDebugDrawer()->drawBox(Rigidbody->getWorldTransform().getOrigin() - m, Rigidbody->getWorldTransform().getOrigin() + m, btVector3(0, 1, 0));
	}
	else if (type == "Sphere") {
		SharedPtr<FSphereShape> box = std::static_pointer_cast<FSphereShape>(Shape);
		btSphereShape* sphere = static_cast<btSphereShape*>(Rigidbody->getCollisionShape());
		btVector3 p(Owner->Transform.Position.x, Owner->Transform.Position.y, Owner->Transform.Position.z);

		FPhysics::btWorld->getDebugDrawer()->drawSphere(p, sphere->getRadius(), btVector3(0,1,0));
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

FObjectComponent* FPhysicsComponent::Clone()
{
	FPhysicsComponent* newComp = new FPhysicsComponent();
	newComp->Mass = Mass;
	newComp->Shape = Shape;
	newComp->IsKinematic = IsKinematic;
	newComp->Restitution = Restitution;
	return newComp;
}

void FPhysicsComponent::SetShape(FCollisionShape* _shape)
{
	
	FPhysics::btWorld->removeRigidBody(Rigidbody.get());
	FPhysics::btWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(Rigidbody->getBroadphaseHandle(), FPhysics::btWorld->getDispatcher());
	Shape = SharedPtr<FCollisionShape>(_shape);
	Rigidbody.reset();
	Rigidbody = nullptr;
	RegisterWithPhysics();
}

void FPhysicsComponent::RegisterWithPhysics()
{
	btCollisionShape* shape = Shape->GetShape();
	if (shape == nullptr) {
		shape = new btBoxShape(btVector3(1,1,1));
	}
	auto pos = Owner->Transform.Position;
	auto rot = Owner->Transform.Rotation;
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(pos.x,pos.y,pos.z));
	btQuaternion r(Owner->Transform.Orientation.x, Owner->Transform.Orientation.y, Owner->Transform.Orientation.z, Owner->Transform.Orientation.w);
	//r.setEulerZYX(rot.y, rot.x, rot.z);
	t.setRotation(r);
	btDefaultMotionState* motionState = new btDefaultMotionState(
		t
	);

	btVector3 inertia(0,0,0);
	if (Mass != 0.0f)
		shape->calculateLocalInertia(Mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		Mass,
		motionState,
		shape,
		inertia
	);
	btRigidBody* rb = new btRigidBody(rigidBodyCI);
	rb->setUserPointer(Owner);
	Rigidbody = SharedPtr<btRigidBody>(rb);
	Rigidbody->activate(true);
	FPhysics::btWorld->addRigidBody(rb);
}

btCollisionShape* FBoxShape::GetShape()
{
	return new btBoxShape(btVector3( Size.x, Size.y, Size.z ));
}

btCollisionShape* FSphereShape::GetShape()
{
	return new btSphereShape(Radius);
}

FCollisionShape::FCollisionShape()
{
}
