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

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/IO/Log.h>

#include "Billiards.h"
#include "Table.h"
#include "Ball.h"
#include "WhiteBall.h"
#include "Interface.h"

#include <Urho3D/DebugNew.h>

#include "iostream"

URHO3D_DEFINE_APPLICATION_MAIN(Billiards)

const float CAMERA_DISTANCE = 20.0f;
// Mouse sensitivity as degrees per pixel
const float MOUSE_SENSITIVITY = 0.1f;


Billiards::Billiards(Context *context) :
        Sample(context) {
    Table::RegisterObject(context);
    Ball::RegisterObject(context);
    WhiteBall::RegisterObject(context);
    Interface::RegisterObject(context);
}

// http://billiards.colostate.edu/threads/physics.html

void Billiards::Start() {
    // Execute base class startup
    Sample::Start();

    // Create the scene content
    CreateScene();

    CreateTable();

    // Create the white ball
    CreateWhiteBall();

    CreateBalls();

    // Create the UI content
    CreateInterface();

    // Setup the viewport for displaying the scene
    SetupViewport();

    // Hook up to the frame update events
    SubscribeToEvents();

    // Set the mouse mode to use in the sample
    Sample::InitMouseMode(MM_RELATIVE);
}

void Billiards::CreateScene() {
    ResourceCache *cache = GetSubsystem<ResourceCache>();


    // Create camera and define viewport. We will be doing load / save, so it's convenient to create the camera outside the scene,
    // so that it won't be destroyed and recreated, and we don't have to redefine the viewport on load
    cameraNode_ = new Node(context_);
    cameraNode_->SetPosition(Vector3(-30.0f, 17.0f, -17.0f));
    Camera *camera = cameraNode_->CreateComponent<Camera>();
    camera->SetFarClip(500.0f);
    GetSubsystem<Renderer>()->SetViewport(0, new Viewport(context_, scene_, camera));

    scene_ = new Scene(context_);

    // Create the Octree component to the scene. This is required before adding any drawable components, or else nothing will
    // show up. The default octree volume will be from (-1000, -1000, -1000) to (1000, 1000, 1000) in world coordinates; it
    // is also legal to place objects outside the volume but their visibility can then not be checked in a hierarchically
    // optimizing manner
    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<PhysicsWorld>();
    scene_->CreateComponent<DebugRenderer>();

    // Create a child scene node (at world origin) and a StaticModel component into it. Set the StaticModel to show a simple
    // plane mesh with a "stone" material. Note that naming the scene nodes is optional. Scale the scene node larger
    // (100 x 100 world units)
    Node *planeNode = scene_->CreateChild("Plane");
    planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
    StaticModel *planeObject = planeNode->CreateComponent<StaticModel>();
    planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
    planeObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));


    // Create a directional light to the world so that we can see something. The light scene node's orientation controls the
    // light direction; we will use the SetDirection() function which calculates the orientation from a forward direction vector.
    // The light will use default settings (white light, no shadows)
    Node *lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetPosition(Vector3(0.0f, 30.0f, 0.0f));
    lightNode->SetDirection(Vector3::DOWN); // The direction vector does not need to be normalized
    Light *light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
}

void Billiards::CreateTable() {
    Node *tableNode = scene_->CreateChild("Table");
    tableNode->SetPosition(Vector3(0.0f, 7.5f, 0.0f));
    tableNode->SetScale(0.1f);

    table_ = tableNode->CreateComponent<Table>();
    table_->Init();
}


void Billiards::CreateWhiteBall() {
    Node *whiteBallNode = scene_->CreateChild("WhiteBall");
    whiteBallNode->SetPosition(Vector3(-10.0f, 7.5f, -1.0f));

    // Create the vehicle logic component
    whiteBall_ = whiteBallNode->CreateComponent<WhiteBall>();
    // Create the rendering and physics components
    whiteBall_->Init(cameraNode_);
}

void Billiards::CreateBalls() {
    // Test ball:
    // CreateBall("Ball1", Vector2(-12.0f, -3.0f));
    CreateBall("Ball1", Vector2(7.0f, -1.0f));
    CreateBall("Ball2", Vector2(8.0f, -1.5f));
    CreateBall("Ball3", Vector2(8.0f, -0.5f));
    CreateBall("Ball4", Vector2(9.0f, 0.f));
    CreateBall("Ball5", Vector2(9.0f, -1.0f));
    CreateBall("Ball6", Vector2(9.0f, -2.0f));
    CreateBall("Ball7", Vector2(10.0f, 0.5f));
    CreateBall("Ball8", Vector2(10.0f, -0.5f));
    CreateBall("Ball9", Vector2(10.0f, -1.5f));
    CreateBall("Ball10", Vector2(10.0f, -2.5f));
    CreateBall("Ball11", Vector2(11.0f, 1.f));
    CreateBall("Ball12", Vector2(11.0f, 0.f));
    CreateBall("Ball13", Vector2(11.0f, -1.f));
    CreateBall("Ball14", Vector2(11.0f, -2.f));
    CreateBall("Ball15", Vector2(11.0f, -3.f));
}

void Billiards::CreateBall(String name, const Vector2 &position) {
    Node *ballNode = scene_->CreateChild(name);
    ballNode->SetPosition(Vector3(position.x_, 7.5f, position.y_));

    Ball *ball = ballNode->CreateComponent<Ball>();
    ball->Init("Materials/Red.xml");
    balls_.Push(WeakPtr<Ball>(ball));
}

void Billiards::CreateInterface() {
    interface_ = GetSubsystem<UI>()->GetRoot()->CreateChild<Interface>();
    interface_->Init();
}


void Billiards::SetupViewport() {
    Renderer *renderer = GetSubsystem<Renderer>();

    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen. We need to define the scene and the camera
    // at minimum. Additionally we could configure the viewport screen size and the rendering path (eg. forward / deferred) to
    // use, but now we just use full screen and default render path configured in the engine command line options
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
}

// For debugging purposes only
void Billiards::SpawnObject() {
    ResourceCache *cache = GetSubsystem<ResourceCache>();

    // Create a smaller box at camera position
    Node *boxNode = scene_->CreateChild("SpawnedBall");
    boxNode->SetPosition(cameraNode_->GetPosition());
    boxNode->SetRotation(cameraNode_->GetRotation());
    boxNode->SetScale(BALL_SCALE);
    StaticModel *boxObject = boxNode->CreateComponent<StaticModel>();
    boxObject->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));
    boxObject->SetMaterial(cache->GetResource<Material>("Materials/StoneEnvMapSmall.xml"));
    boxObject->SetCastShadows(true);

    // Create physics components, use a smaller mass also
    RigidBody *body = boxNode->CreateComponent<RigidBody>();
    body->SetMass(BALL_MASS);
    body->SetFriction(BALL_FRICTION);
    body->SetLinearDamping(BALL_LINEAR_DAMPING);
    body->SetAngularDamping(BALL_ANGULAR_DAMPING);
//    body->SetRollingFriction(1.0f);
    body->SetRestitution(BALL_RESTITUTION);
    CollisionShape *shape = boxNode->CreateComponent<CollisionShape>();
    shape->SetSphere(1.0f);

    const float OBJECT_VELOCITY = 10.0f;

    // Set initial velocity for the RigidBody based on camera forward vector. Add also a slight up component
    // to overcome gravity better
    body->SetLinearVelocity(cameraNode_->GetRotation() * Vector3(0.0f, 0.25f, 1.0f) * OBJECT_VELOCITY);
}


// TODO: use this in free camera mode
void Billiards::MoveCamera(float timeStep) {
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
        return;

    Input *input = GetSubsystem<Input>();

    // Movement speed as world units per second
    const float MOVE_SPEED = 20.0f;

    // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    IntVector2 mouseMove = input->GetMouseMove();
    yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
    pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
    pitch_ = Clamp(pitch_, -90.0f, 90.0f);

    // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    // Use the Translate() function (default local space) to move relative to the node's orientation.
    if (input->GetKeyDown(KEY_W))
        cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_S))
        cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_A))
        cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_D))
        cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);

}


void Billiards::SubscribeToEvents() {
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Billiards, HandleUpdate));

    // Subscribe to PostUpdate event for updating the camera position after physics simulation
    SubscribeToEvent(E_POSTUPDATE, URHO3D_HANDLER(Billiards, HandlePostUpdate));

    // Subscribe HandlePostRenderUpdate() function for processing the post-render update event, during which we request
    // debug geometry
    SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(Billiards, HandlePostRenderUpdate));

    SubscribeToEvent(E_BALLINPOCKET, URHO3D_HANDLER(Billiards, HandleBallInPocket));
}

void Billiards::HandleBallInPocket(StringHash eventType, VariantMap &eventData) {
    // remove ball from balls_ vector
    String ballName = eventData[BallInPocket::P_BALLNAME].GetString();

    for (Vector<WeakPtr<Ball>>::Iterator it = balls_.Begin(); it != balls_.End(); ++it) {
        if ((*it)->GetName() == ballName) {
            balls_.Remove(*it);
            break;
        }
    }
}

void Billiards::HandleUpdate(StringHash eventType, VariantMap &eventData) {
    using namespace Update;

    Input *input = GetSubsystem<Input>();

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Before any next calculations - update isAnyBallMoving variable
    IsAnyBallMoving();

    if (!isAnyBallMoving_ && balls_.Empty()) {
        // no update anymore - show win screen
        interface_->ShowWinScreen();
        UnsubscribeFromEvent(E_UPDATE);
        UnsubscribeFromEvent(E_POSTUPDATE);
        UnsubscribeFromEvent(E_BALLINPOCKET);
        return;
    }


    if (whiteBall_) {
        // handle changing camera mode
        if (input->GetKeyPress(KEY_C)) {
            if (!cameraFreeMode_) {
                // if camera is switch to free mode - assign whiteBall controls to camera
                yaw_ = whiteBall_->controls_.yaw_;
                pitch_ = whiteBall_->controls_.pitch_;
            }
            cameraFreeMode_ = !cameraFreeMode_;
        }

        UI *ui = GetSubsystem<UI>();

        // Get movement controls and assign them to the vehicle component. If UI has a focused element, clear controls
        if (!ui->GetFocusElement()) {

            if (!isAnyBallMoving_) {
                // allow to push ball only after all balls stopped
                whiteBall_->controls_.Set(CTRL_PUSH, input->GetKeyDown(KEY_SPACE));
            }

            if (!cameraFreeMode_) {
                whiteBall_->controls_.yaw_ += (float) input->GetMouseMoveX() * MOUSE_SENSITIVITY;
                whiteBall_->controls_.pitch_ += (float) input->GetMouseMoveY() * MOUSE_SENSITIVITY;
                // Limit pitch
                whiteBall_->controls_.pitch_ = Clamp(whiteBall_->controls_.pitch_, 10.0f, 70.0f);
            }
        } else {
            whiteBall_->controls_.Set(CTRL_PUSH, 0);
        }
    } else {
        cameraFreeMode_ = true;
    }

    // Move the camera, scale movement with time step
    if (cameraFreeMode_) {
        MoveCamera(timeStep);
    }
}

void Billiards::HandlePostUpdate(StringHash eventType, VariantMap &eventData) {
    if (!whiteBall_)
        return;

    Node *whiteBallNode = whiteBall_->GetNode();

    if (!cameraFreeMode_) {
        Quaternion dir(whiteBall_->controls_.yaw_, Vector3::UP);
        dir = dir * Quaternion(whiteBall_->controls_.pitch_, Vector3::RIGHT);

        Vector3 cameraTargetPos = whiteBallNode->GetPosition() - dir * Vector3(0.0f, 0.0f, CAMERA_DISTANCE);
        Vector3 cameraStartPos = whiteBallNode->GetPosition();

        // Raycast camera against static objects (physics collision mask 2)
        // and move it closer to the vehicle if something in between
        Ray cameraRay(cameraStartPos, cameraTargetPos - cameraStartPos);
        float cameraRayLength = (cameraTargetPos - cameraStartPos).Length();
        PhysicsRaycastResult result;
        scene_->GetComponent<PhysicsWorld>()->RaycastSingle(result, cameraRay, cameraRayLength, 2);
        if (result.body_)
            cameraTargetPos = cameraStartPos + cameraRay.direction_ * (result.distance_ - 0.5f);

        cameraNode_->SetPosition(cameraTargetPos);
        cameraNode_->SetRotation(dir);
    }

    if (!isAnyBallMoving_) {
        // set push level
        const int level = int(
                whiteBall_->pushButtonHoldingTime_ / MAX_PUSH_BUTTON_HOLD_TIME * PUSH_FORCE_LEVEL_BAR_DOTS_COUNT
        );
        interface_->UpdatePushLevel(level);
    }
    interface_->SetStatusText(isAnyBallMoving_);
}

void Billiards::HandlePostRenderUpdate(StringHash eventType, VariantMap &eventData) {
    // If draw debug mode is enabled, draw physics debug geometry. Use depth test to make the result easier to interpret
    //    if (drawDebug_)
    scene_->GetComponent<PhysicsWorld>()->DrawDebugGeometry(true);
}

bool Billiards::IsAnyBallMoving() {
    isAnyBallMoving_ = false;

    if (whiteBall_->IsMoving()) {
        // if white ball is moving - do not even check other balls
        isAnyBallMoving_ = true;
    } else {
        for (Vector<WeakPtr<Ball>>::Iterator it = balls_.Begin(); it != balls_.End(); ++it) {
            WeakPtr<Ball> ball = (*it);
            if (ball && ball->IsMoving()) {
                isAnyBallMoving_ = true;
                break;
            }
        }

    }
    return isAnyBallMoving_;
}
