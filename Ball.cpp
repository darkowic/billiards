#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Physics/PhysicsEvents.h>

#include "Ball.h"

#include <Urho3D/IO/Log.h>


Ball::Ball(Context *context) : LogicComponent(context) {
    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

void Ball::RegisterObject(Context *context) {
    context->RegisterFactory<Ball>();
}

void Ball::Start() {
    // Component has been inserted into its scene node. Subscribe to events now
    SubscribeToEvent(node_, E_NODECOLLISIONSTART, URHO3D_HANDLER(Ball, HandleNodeCollisionStart));
}

String Ball::GetName() {
    return node_->GetName();
}

void Ball::Init(String material) {

    // This function is called only from the main program when initially creating the vehicle, not on scene load
    ResourceCache *cache = GetSubsystem<ResourceCache>();

    // create ball structure
    StaticModel *ballObject = node_->CreateComponent<StaticModel>();
    body_ = node_->CreateComponent<RigidBody>();
    CollisionShape *shape = node_->CreateComponent<CollisionShape>();

    // set scale
    node_->SetScale(BALL_SCALE);

    ballObject->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));

    ballObject->SetMaterial(cache->GetResource<Material>(material));

    shape->SetSphere(1.0f);
    body_->SetMass(BALL_MASS);
    body_->SetFriction(BALL_FRICTION);
    body_->SetLinearDamping(BALL_LINEAR_DAMPING);
    body_->SetAngularDamping(BALL_ANGULAR_DAMPING);
//    body->SetRollingFriction(1.0f);
    body_->SetRestitution(BALL_RESTITUTION);
}

bool Ball::IsMoving() {
    return body_->GetLinearVelocity() != Vector3::ZERO;
};

void Ball::HandleNodeCollisionStart(StringHash eventType, VariantMap &eventData) {
    if (IsCollidingWithPocket(eventData)) {
        HandleCollisionWithPocket(eventData);
    }
}


bool Ball::IsCollidingWithPocket(VariantMap &eventData) {
    Node *otherNode = static_cast<Node *>(eventData[NodeCollision::P_OTHERNODE].GetPtr());
    if (otherNode->HasTag("pocket")) {
        return true;
    }
    return false;
}

void Ball::HandleCollisionWithPocket(VariantMap &eventData) {
    VariantMap data;
    data[BallInPocket::P_BALLNAME] = GetName();
    SendEvent(E_BALLINPOCKET, data);
    node_->Remove();
    Remove();
}