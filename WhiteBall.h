#pragma once

#include <Urho3D/Input/Controls.h>

#include "Ball.h"


class WhiteBall : public Ball {

public:
    /// Movement controls.
    Controls controls_;
};
