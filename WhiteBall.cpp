#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Physics/RigidBody.h>

#include <Urho3D/IO/Log.h>

#include "WhiteBall.h"


WhiteBall::WhiteBall(Context *context) : Ball(context) {
    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

void WhiteBall::RegisterObject(Context* context)
{
    context->RegisterFactory<WhiteBall>();
}


void WhiteBall::Init(WeakPtr<Node> cameraNode) {
    Ball::Init();
    cameraNode_ = cameraNode;
}


void WhiteBall::FixedUpdate(float timeStep)
{

    const float PUSH_FORCE = 50.0f;

    // Read controls
    if (controls_.buttons_ & CTRL_PUSH) {
        URHO3D_LOGINFO("Click space and push");

        Vector3 moveDirection = (node_->GetPosition() - cameraNode_->GetPosition()).Normalized();

        body_->ApplyForce(moveDirection * PUSH_FORCE);
    }
}

