#pragma once

#include <Urho3D/Scene/LogicComponent.h>

namespace Urho3D {
    class RigidBody;
}

using namespace Urho3D;

const float BALL_SCALE = 0.7f;
const float BALL_MASS = 1.0f;
const float BALL_FRICTION = 0.2f;
const float BALL_LINEAR_DAMPING = 0.5f;
const float BALL_ANGULAR_DAMPING = 0.5f;
const float BALL_RESTITUTION = 0.5f;


class Ball : public LogicComponent {
URHO3D_OBJECT(Ball, LogicComponent);

public:
    /// Construct.
    Ball(Context *context);

    /// Register object factory and attributes.
    static void RegisterObject(Context *context);

    /// Initialize ball
    void Init();

protected:
    WeakPtr<RigidBody> body_;
};
