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
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

void Ball::Start() {
    // Component has been inserted into its scene node. Subscribe to events now
    SubscribeToEvent(E_PHYSICSCOLLISIONSTART, URHO3D_HANDLER(Ball, HandlePhysicsCollisionStart));
}


void Ball::RegisterObject(Context *context) {
    context->RegisterFactory<Ball>();
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

void Ball::HandlePhysicsCollisionStart(StringHash eventType, VariantMap &eventData) {

    using namespace PhysicsCollision;

    Node *nodeA = static_cast<Node *>(eventData[P_NODEA].GetPtr());
    Node *nodeB = static_cast<Node *>(eventData[P_NODEB].GetPtr());
    if (nodeA->HasTag("pocket") || nodeB->HasTag("pocket")) {
        URHO3D_LOGINFO("Collition with Pocket!!!!!!!! OMG OMG");
    }
}
