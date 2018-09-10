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

void WhiteBall::RegisterObject(Context *context) {
    context->RegisterFactory<WhiteBall>();
}


void WhiteBall::Init(WeakPtr<Node> cameraNode) {
    Ball::Init();
    cameraNode_ = cameraNode;
}


void WhiteBall::FixedUpdate(float timeStep) {

    if ((controls_.buttons_ & CTRL_PUSH)) {
        if (pushButtonHoldingTime_ < MAX_PUSH_BUTTON_HOLD_TIME) {
            pushButtonHoldingTime_ += timeStep;
        }
    } else {
        if (pushButtonHoldingTime_ >= timeStep) {
            URHO3D_LOGINFO("Push the ball!" + (String) pushButtonHoldingTime_);

            Vector3 moveDirection = (node_->GetPosition() - cameraNode_->GetPosition()).Normalized();

            body_->ApplyForce(moveDirection * PUSH_FORCE_PER_SECOND * pushButtonHoldingTime_);
        }
        // reset holding time value
        pushButtonHoldingTime_ = 0.f;
    }
}

