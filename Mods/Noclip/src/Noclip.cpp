#include <IconsMaterialDesign.h>

#include <Glacier/ZGameLoopManager.h>
#include <Glacier/Input/ZInputActionManager.h>
#include <Glacier/ZLevelManager.h>

#include <Noclip.h>
#include <Hooks.h>

Noclip::Noclip() :
    areBindingsAdded(false),
    isNoclipEnabled(false),
    toggleNoclipAction("ToggleNoclip")
{
}

Noclip::~Noclip()
{
    const ZMemberDelegate<Noclip, void(const SGameUpdateEvent&)> delegate(this, &Noclip::OnFrameUpdate);

    GameLoopManager->UnregisterForFrameUpdate(delegate);
}

void Noclip::OnEngineInitialized()
{
    const ZMemberDelegate<Noclip, void(const SGameUpdateEvent&)> delegate(this, &Noclip::OnFrameUpdate);

    GameLoopManager->RegisterForFrameUpdate(delegate, 1);
}

void Noclip::OnDrawMenu()
{
    if (ImGui::Checkbox(ICON_MD_SELF_IMPROVEMENT " Noclip", &isNoclipEnabled))
    {
        if (isNoclipEnabled)
        {
            ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

            if (hitman)
            {
                playerPosition = hitman->GetSpatialEntityPtr()->GetWorldPosition();
            }
        }
    }
}

void Noclip::OnFrameUpdate(const SGameUpdateEvent& updateEvent)
{
    if (!areBindingsAdded)
    {
        const char* bindings = "NoclipInput={"
            "ToggleNoclip=tap(kb,n);};";

        InputActionManager->AddBindings(bindings);

        areBindingsAdded = true;
    }

    ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

    if (!hitman)
    {
        return;
    }

    if (toggleNoclipAction.Digital())
    {
        isNoclipEnabled = !isNoclipEnabled;

        if (isNoclipEnabled)
        {
            playerPosition = hitman->GetSpatialEntityPtr()->GetWorldPosition();
        }
    }

    if (!isNoclipEnabled)
    {
        return;
    }

    ZHM5MainCamera* mainCamera = hitman->GetMainCamera();

    if (!mainCamera)
    {
        return;
    }

    const SMatrix cameraTransform = mainCamera->GetObjectToWorldMatrix();
    float moveSpeed = 5.f;

    if (GetAsyncKeyState(VK_SHIFT))
        moveSpeed = 20.f;

    if (GetAsyncKeyState('W'))
        playerPosition += cameraTransform.Up * -moveSpeed * updateEvent.m_GameTimeDelta.ToSeconds();

    if (GetAsyncKeyState('S'))
        playerPosition += cameraTransform.Up * moveSpeed * updateEvent.m_GameTimeDelta.ToSeconds();

    if (GetAsyncKeyState('A'))
        playerPosition += cameraTransform.Left * -moveSpeed * updateEvent.m_GameTimeDelta.ToSeconds();

    if (GetAsyncKeyState('D'))
        playerPosition += cameraTransform.Left * moveSpeed * updateEvent.m_GameTimeDelta.ToSeconds();

    hitman->GetSpatialEntityPtr()->SetWorldPosition(playerPosition);
}

DEFINE_MOD(Noclip);
