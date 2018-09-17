#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "Interface.h"


Interface::Interface(Context *context) :
        UIElement(context),
        uiRoot_(GetSubsystem<UI>()->GetRoot()) {}


void Interface::RegisterObject(Context *context) {
    context->RegisterFactory<Interface>();
}

void Interface::Init() {
    ResourceCache *cache = GetSubsystem<ResourceCache>();

    UIElement *pushForceLevelBarContainer = uiRoot_->CreateChild<UIElement>();
    pushForceLevelBarContainer->SetPosition(0, 0);

    // Construct new Text object, set string to display and font to use
    Text *instructionText = uiRoot_->CreateChild<Text>();
    instructionText->SetText(
            "Put all red balls in pockets using white ball\n"
            "Use mouse to rotate camera\n"
            "C to toggle camera mode\n"
            "X to toggle physics debug geometry"
    );
    instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
    // The text has multiple rows. Center them in relation to each other
    instructionText->SetTextAlignment(HA_CENTER);

    // Position the text relative to the screen center
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_BOTTOM);
    instructionText->SetPosition(0, -instructionText->GetHeight() - 10);

    Text *pushForceLevelBarTipText = pushForceLevelBarContainer->CreateChild<Text>();
    pushForceLevelBarTipText->SetText("Push force level bar");
    pushForceLevelBarTipText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);

    UIElement *barContainer = pushForceLevelBarContainer->CreateChild<UIElement>();
    barContainer->SetPosition(0, 25);
    Text *barTextStart = barContainer->CreateChild<Text>();
    barTextStart->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
    barTextStart->SetText("|");
    pushForceLevelBarValue_ = barContainer->CreateChild<Text>();
    pushForceLevelBarValue_->SetPosition(barTextStart->GetWidth(), 0);
    pushForceLevelBarValue_->SetText(GetPushForceLevelString(0));
    pushForceLevelBarValue_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);

    Text *barTextEnd = barContainer->CreateChild<Text>();
    barTextEnd->SetPosition(barTextStart->GetWidth() + pushForceLevelBarValue_->GetWidth(), 0);;
    barTextEnd->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
    barTextEnd->SetText("| Max");

    statusText_ = uiRoot_->CreateChild<Text>();
    statusText_->SetPosition(0, 50);
    statusText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
    SetStatusText(false);
}

void Interface::SetStatusText(bool isMoving) {
    if (isMoving) {
        statusText_->SetText("Balls are rolling...");
    } else {
        statusText_->SetText("Hold the space button to push the white ball");
    }
}


void Interface::UpdatePushLevel(int level) {
    pushForceLevelBarValue_->SetText(GetPushForceLevelString(level));
}

void Interface::ShowWinScreen() {
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    Text *winText = uiRoot_->CreateChild<Text>();
    winText->SetText("You win!");
    winText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 25);

    // Position the text relative to the screen center
    winText->SetHorizontalAlignment(HA_CENTER);
    winText->SetVerticalAlignment(VA_CENTER);
}

void Interface::ShowGameOverScreen() {
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    Text *winText = uiRoot_->CreateChild<Text>();
    winText->SetText("Oops! Black ball in pocket... Game over!");
    winText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 25);

    // Position the text relative to the screen center
    winText->SetHorizontalAlignment(HA_CENTER);
    winText->SetVerticalAlignment(VA_CENTER);
}

String Interface::GetPushForceLevelString(int level) {
    String text = "";
    for (int i = 1; i <= PUSH_FORCE_LEVEL_BAR_DOTS_COUNT; ++i) {
        if (i <= level) {
            text += ">";
        } else {
            text += ".";
        }
    }
    return text;
}

void Interface::ShowWhiteBallInPocketInfo() {
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    Text *text = uiRoot_->CreateChild<Text>("whiteBallInPocketInfo");
    text->SetText("Oh! White ball in pocket...\nPlease wait until all balls stop moving...");
    text->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 20);
    text->SetTextAlignment(HA_CENTER);

    // Position the text relative to the screen center
    text->SetHorizontalAlignment(HA_CENTER);
    text->SetVerticalAlignment(VA_CENTER);
}

void Interface::HideWhiteBallInPocketInfo() {
    uiRoot_->GetChild(String("whiteBallInPocketInfo"))->Remove();
}
