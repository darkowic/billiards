#pragma once

#include <Urho3D/Input/Controls.h>

#include "Ball.h"

namespace Urho3D {
    class RigidBody;

    class Node;
}

using namespace Urho3D;


const int CTRL_PUSH = 1;

const float PUSH_FORCE_PER_SECOND = 1500.0f;
const unsigned MAX_PUSH_BUTTON_HOLD_TIME = 2.0f; // 2 seconds???

const float WHITE_BALL_INITIAL_PITH = 17.0f;
const float WHITE_BALL_INITIAL_YAW = 90.0f;


URHO3D_EVENT(E_WHITEBALLINPOCKET, WhiteBallInPocket) {
}


class WhiteBall : public Ball {
URHO3D_OBJECT(WhiteBall, Ball);

public:
    /// Construct.
    WhiteBall(Context *context);

    /// Register object factory and attributes.
    static void RegisterObject(Context *context);

    /// Handle physics world update. Called by LogicComponent base class.
    virtual void FixedUpdate(float timeStep);

    // Gets camera node pointer
    void Init(WeakPtr<Node>);

    /// Movement controls.
    Controls controls_;

    float pushButtonHoldingTime_ = 0.f;

private:
    void HandleCollisionWithPocket(VariantMap &eventData);

    WeakPtr<Node> cameraNode_;
    Vector3 initialPosition_;
};
