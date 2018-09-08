#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/Model.h>

#include "Ball.h"


Ball::Ball(Context *context) : LogicComponent(context) {
    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}


void Ball::RegisterObject(Context* context)
{
    context->RegisterFactory<Ball>();
}

void Ball::Init() {

    // This function is called only from the main program when initially creating the vehicle, not on scene load
    ResourceCache *cache = GetSubsystem<ResourceCache>();

    // create ball structure
    StaticModel* ballObject = node_->CreateComponent<StaticModel>();
    RigidBody *body = node_->CreateComponent<RigidBody>();
    CollisionShape *shape = node_->CreateComponent<CollisionShape>();

    // set scale
    node_->SetScale(BALL_SCALE);

    ballObject->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));

    shape->SetSphere(1.0f);
    body->SetMass(BALL_MASS);
    body->SetFriction(BALL_FRICTION);
    body->SetLinearDamping(BALL_LINEAR_DAMPING);
    body->SetAngularDamping(BALL_ANGULAR_DAMPING);
//    body->SetRollingFriction(1.0f);
    body->SetRestitution(BALL_RESTITUTION);
}
