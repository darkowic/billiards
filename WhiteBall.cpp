#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Physics/RigidBody.h>

#include <Urho3D/IO/Log.h>

#include "WhiteBall.h"


WhiteBall::WhiteBall(Context *context) : Ball(context) {
    // set initial camera position
    controls_.pitch_ = 17.0f;
    controls_.yaw_ = 90.0f;
    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

void WhiteBall::RegisterObject(Context *context) {
    context->RegisterFactory<WhiteBall>();
}


void WhiteBall::Init(WeakPtr<Node> cameraNode) {
    Ball::Init("Materials/White.xml");
    cameraNode_ = cameraNode;
}


void WhiteBall::FixedUpdate(float timeStep) {

    if ((controls_.buttons_ & CTRL_PUSH)) {
        if (pushButtonHoldingTime_ < MAX_PUSH_BUTTON_HOLD_TIME) {
            pushButtonHoldingTime_ += timeStep;
        }
    } else {
        if (!Urho3D::Equals(pushButtonHoldingTime_, 0.f)) {
            URHO3D_LOGINFO("Push the ball!" + (String) pushButtonHoldingTime_);

            Vector3 moveDirection = (node_->GetPosition() - cameraNode_->GetPosition()).Normalized();
            // project the moveDirection vector to x z -> set y to 0
            moveDirection.y_ = 0.f;
            body_->ApplyForce(moveDirection * PUSH_FORCE_PER_SECOND * pushButtonHoldingTime_);
        }
        // reset holding time value
        pushButtonHoldingTime_ = 0.f;
    }
}

