#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>

#include "Table.h"


Table::Table(Context *context) : LogicComponent(context) {
    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}


void Table::RegisterObject(Context *context) {
    context->RegisterFactory<Table>();
}

void Table::Init() {

    // This function is called only from the main program when initially creating the vehicle, not on scene load
    ResourceCache *cache = GetSubsystem<ResourceCache>();

    // create Table structure
    StaticModel *tableObject = node_->CreateComponent<StaticModel>();
    RigidBody *body = node_->CreateComponent<RigidBody>();
    CollisionShape *shape = node_->CreateComponent<CollisionShape>();

    tableObject->SetModel(cache->GetResource<Model>("Models/Table.mdl"));
    tableObject->SetMaterial(0, cache->GetResource<Material>("Materials/01 - Default.xml"));
    tableObject->SetMaterial(1, cache->GetResource<Material>("Materials/08 - Default.xml"));
    tableObject->SetMaterial(2, cache->GetResource<Material>("Materials/07 - Default.xml"));
    tableObject->SetMaterial(3, cache->GetResource<Material>("Materials/02 - Default.xml"));
    tableObject->SetMaterial(4, cache->GetResource<Material>("Materials/03 - Default.xml"));

    CollisionShape *tableFloor = node_->CreateComponent<CollisionShape>();
    tableFloor->SetBox(Vector3(333.0f, 0.0f, 160.0f));
    tableFloor->SetPosition(Vector3(10.0f, -4.8f, -5.0f));
    body->SetRestitution(0.2f);
    body->SetFriction(0.2f);

    Node *bumpersNode = node_->CreateChild("Bumpers");
    RigidBody *bumpersBody = bumpersNode->CreateComponent<RigidBody>();
    bumpersBody->SetRestitution(0.5f);

    InitBumper(bumpersNode, Vector3(155.0f, 5.0f, 5.0f), Vector3(-75.0f, -1.5f, 79.5f));
    InitBumper(bumpersNode, Vector3(155.0f, 5.0f, 5.0f), Vector3(96.0f, -1.5f, 79.5f));
    InitBumper(bumpersNode, Vector3(155.0f, 5.0f, 5.0f), Vector3(-75.0f, -1.5f, -89.0f));
    InitBumper(bumpersNode, Vector3(155.0f, 5.0f, 5.0f), Vector3(96.0f, -1.5f, -89.0f));
    InitBumper(bumpersNode, Vector3(5.0f, 5.0f, 148.0f), Vector3(181.0f, -1.5f, -5.0f));
    InitBumper(bumpersNode, Vector3(5.0f, 5.0f, 148.0f), Vector3(-160.0f, -1.5f, -5.0f));
}


void Table::InitBumper(Node* node, const Vector3 &size, const Vector3 &position) {
    CollisionShape *bumper = node->CreateComponent<CollisionShape>();
    bumper->SetBox(size);
    bumper->SetPosition(position);
}