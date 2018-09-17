#pragma once

#include <Urho3D/UI/UIElement.h>

namespace Urho3D {

    class UI;

    class String;

    class Text;

}

using namespace Urho3D;


const int PUSH_FORCE_LEVEL_BAR_DOTS_COUNT = 25;

class Interface : public UIElement {
URHO3D_OBJECT(Interface, UIElement);

public:
    Interface(Context *context);

    static void RegisterObject(Context *context);

    void Init();

    void UpdatePushLevel(int level);

    void SetStatusText(bool isMoving);

    void ShowWinScreen();

    void ShowGameOverScreen();

    void ShowWhiteBallInPocketInfo();

    void HideWhiteBallInPocketInfo();

private:
    String GetPushForceLevelString(int);

    WeakPtr<UIElement> uiRoot_;
    WeakPtr<Text> pushForceLevelBarValue_;
    WeakPtr<Text> statusText_;
};
