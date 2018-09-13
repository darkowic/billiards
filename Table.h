#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;


class Table : public LogicComponent {
URHO3D_OBJECT(Table, LogicComponent);

public:
    /// Construct.
    Table(Context *context);

    /// Register object factory and attributes.
    static void RegisterObject(Context *context);

    /// Initialize ball
    void Init();

private:
    void InitBumper(Node *node, const Vector3 &size, const Vector3 &position);

    void InitPocket(String name, const Vector3 &position);
};
