#pragma once

#include <Urho3D/Input/Controls.h>

#include "Ball.h"

namespace Urho3D {
    class RigidBody;
    class Node;
}

using namespace Urho3D;


const int CTRL_PUSH = 1;


class WhiteBall : public Ball {
URHO3D_OBJECT(WhiteBall, Ball);

public:
    /// Construct.
    WhiteBall(Context* context);

    /// Register object factory and attributes.
    static void RegisterObject(Context *context);

    /// Handle physics world update. Called by LogicComponent base class.
    virtual void FixedUpdate(float timeStep);

    // Gets camera node pointer
    void Init(WeakPtr<Node>);

    /// Movement controls.
    Controls controls_;

private:
    WeakPtr<Node> cameraNode_;
};
