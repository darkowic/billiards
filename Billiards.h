//
// Copyright (c) 2008-2017 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "Sample.h"

namespace Urho3D {

    class Node;

    class Scene;

    class Text;

}

class Table;

class Ball;

class WhiteBall;

class Interface;

class Billiards : public Sample {
URHO3D_OBJECT(Billiards, Sample);

public:
    /// Construct.
    Billiards(Context *context);

    /// Setup after engine initialization and before running the main loop.
    virtual void Start();

private:
    /// Construct the scene content.
    void CreateScene();

    /// Construct an instruction text to the UI.
    void CreateInterface();

    /// Set up a viewport for displaying the scene.
    void SetupViewport();

    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();

    /// Read input and moves the camera.
    void MoveCamera(float timeStep);

    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap &eventData);


    void HandlePostUpdate(StringHash eventType, VariantMap &eventData);

    /// Handle the post-render update event.
    void HandlePostRenderUpdate(StringHash eventType, VariantMap &eventData);

    void HandleBallInPocket(StringHash eventType, VariantMap &eventData);

    void HandleWhiteBallInPocket(StringHash eventType, VariantMap &eventData);

    void SpawnObject();

    void CreateTable();

    void CreateWhiteBall();

    void CreateBalls();

    void CreateBall(String name, const Vector2 &position, String material = "Materials/Red.xml");

    bool IsAnyBallMoving();

    WeakPtr<Table> table_;
    WeakPtr<WhiteBall> whiteBall_;
    WeakPtr<Interface> interface_;
    bool isAnyBallMoving_ = false;
    Vector<WeakPtr<Ball>> balls_;
    bool cameraFreeMode_ = false;
    bool whiteBallInPocket_ = false;
    bool drawDebug_ = false;
};
