#include "imgui.h"

#include "IconsMaterialDesign.h"

#include <Glacier/Templates/TEntityRef.h>
#include <Glacier/Render/IRenderDestinationEntity.h>
#include <Glacier/Render/ZRenderManager.h>
#include <Glacier/ZLevelManager.h>
#include <Glacier/Engine/ZApplicationEngineWin32.h>
#include <Glacier/Input/ZInputActionManager.h>
#include <Glacier/Math/ZMath.h>
#include <Glacier/Physics/ZCollisionManager.h>
#include <Glacier/Actor/ZActor.h>

#include <Glacier/Module/ZHitman5Module.h>

#include "FreeCamera.h"
#include "Global.h"
#include "SDK.h"
#include "Hooks.h"
#include "Function.h"

FreeCamera::FreeCamera() :
    areBindingsAdded(false),
    isFreeCameraActive(false),
    freezeCamera(false),
    toggleFreeCameraAction("ToggleFreeCamera"),
    freezeFreeCameraAction("FreezeCamera"),
    instantlyKillNpcAction("InstantKill"),
    teleportMainCharacterAction("Teleport"),
    areControlsVisible(false)
{
    pcControls = {
        { "K", "Toggle freecam" },
        { "F3", "Lock camera and enable 47 input" },
        { "Ctrl + W/S", "Change FOV" },
        { "Ctrl + A/D", "Roll camera" },
        { "Alt + W/S", "Change camera speed" },
        { "Space + Q/E", "Change camera height" },
        { "Space + W/S", "Move camera on axis" },
        { "Shift", "Increase camera speed" },
        { "Ctrl + F9", "Teleport Hitman" },
        { "F9", "Kill NPC" },
    };

    controllerControls = {
        { "Y + L", "Change FOV" },
        { "A + L", "Roll camera" },
        { "A + L press", "Reset rotation" },
        { "B + R", "Change camera speed" },
        { "RT", "Increase camera speed" },
        { "LB", "Lock camera and enable 47 input" },
        { "LT + R", "Change camera height" },
    };
}

FreeCamera::~FreeCamera()
{
    const ZMemberDelegate<FreeCamera, void(const SGameUpdateEvent&)> delegate(this, &FreeCamera::OnFrameUpdate);

    GameLoopManager->UnregisterForFrameUpdate(delegate);

    if (isFreeCameraActive)
    {
        ZApplicationEngineWin32* applicationEngineWin32 = ZApplicationEngineWin32::GetInstance();
        ZEngineAppCommon& engineAppCommon = applicationEngineWin32->GetEngineAppCommon();
        TEntityRef<IRenderDestinationEntity> renderDestinationEntity = RenderManager->GetGameRenderDestinationEntity();

        renderDestinationEntity.GetRawPointer()->SetSource(engineAppCommon.GetMainCamera().GetEntityRef());

        TEntityRef<ZHitman5> hitman = LevelManager->GetHitman();

        if (hitman.GetRawPointer())
        {
            ZHM5InputControl* inputControl = hitman.GetRawPointer()->GetInputControl();

            if (inputControl)
            {
                inputControl->EnableBindings();
            }
        }
    }
}

void FreeCamera::OnEngineInitialized()
{
    const ZMemberDelegate<FreeCamera, void(const SGameUpdateEvent&)> delegate(this, &FreeCamera::OnFrameUpdate);

    GameLoopManager->RegisterForFrameUpdate(delegate, 1);
}

void FreeCamera::OnDrawMenu()
{
    bool isFreeCameraActive = this->isFreeCameraActive;

    if (ImGui::Checkbox(ICON_MD_PHOTO_CAMERA " Free Camera", &isFreeCameraActive))
    {
        ToggleFreeCamera();
    }

    if (ImGui::Button(ICON_MD_SPORTS_ESPORTS " Free Camera Controls"))
    {
        areControlsVisible = !areControlsVisible;
    }
}

void FreeCamera::OnDrawUI(const bool hasFocus)
{
    if (areControlsVisible)
    {
        ImGui::PushFont(SDK::GetInstance().GetBoldFont());

        const auto areControlsExpanded = ImGui::Begin(ICON_MD_PHOTO_CAMERA " Free Camera Controls", &areControlsVisible);

        ImGui::PushFont(SDK::GetInstance().GetRegularFont());

        if (areControlsExpanded)
        {
            ImGui::TextUnformatted("PC Controls");
            ImGui::BeginTable("FreeCameraControlsPc", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit);

            for (auto& [key, description] : pcControls)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(key.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(description.c_str());
            }

            ImGui::EndTable();

            ImGui::TextUnformatted("Controller Controls");
            ImGui::BeginTable("FreeCameraControlsController", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit);

            for (auto& [key, description] : controllerControls)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(key.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(description.c_str());
            }

            ImGui::EndTable();
        }

        ImGui::PopFont();
        ImGui::End();
        ImGui::PopFont();
    }
}

IAllocator* GetNormalAllocator()
{
    return Function::CallAndReturn<IAllocator*>(BaseAddress + 0x2F24A0);
}

void FreeCamera::OnFrameUpdate(const SGameUpdateEvent& p_UpdateEvent)
{
    if (!areBindingsAdded)
    {
        const char* bindings = "FreeCameraInput={"
            "ToggleFreeCamera=tap(kb,k);"
            "Teleport=& | hold(kb,lctrl) hold(kb,rctrl) tap(kb,f9);"
            "InstantKill=tap(kb,f9);"
            "FreezeCamera=tap(kb,f3);};";

        InputActionManager->AddBindings(bindings);

        areBindingsAdded = true;
    }

    ZApplicationEngineWin32* applicationEngineWin32 = ZApplicationEngineWin32::GetInstance();

    if (!applicationEngineWin32)
    {
        return;
    }

    ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

    if (!hitman)
    {
        return;
    }

    ZEngineAppCommon& engineAppCommon = applicationEngineWin32->GetEngineAppCommon();
    ZFreeCameraControlEntity* freeCameraControlEntity = engineAppCommon.GetFreeCameraControl().GetRawPointer();

    if (toggleFreeCameraAction.Digital())
    {
        ToggleFreeCamera();
    }

    if (isFreeCameraActive)
    {
        if (instantlyKillNpcAction.Digital())
        {
            InstantlyKillNpc();
        }

        if (teleportMainCharacterAction.Digital())
        {
            TeleportMainCharacter();
        }

        if (freezeFreeCameraAction.Digital())
        {
            freezeCamera = !freezeCamera;

            if (freeCameraControlEntity)
            {
                freeCameraControlEntity->SetActive(!freezeCamera);
            }

            ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

            if (hitman)
            {
                ZHM5InputControl* inputControl = hitman->GetInputControl();

                if (inputControl)
                {
                    if (freezeCamera)
                    {
                        inputControl->EnableBindings();
                    }
                    else
                    {
                        inputControl->DisableBindings();
                    }
                }
            }
        }
    }
}

void FreeCamera::ToggleFreeCamera()
{
    ZApplicationEngineWin32* applicationEngineWin32 = ZApplicationEngineWin32::GetInstance();
    ZEngineAppCommon& engineAppCommon = applicationEngineWin32->GetEngineAppCommon();

    if (!engineAppCommon.GetFreeCamera().GetRawPointer())
    {
        engineAppCommon.CreateFreeCameraAndControl();
    }

    isFreeCameraActive = !isFreeCameraActive;

    if (isFreeCameraActive)
    {
        EnableFreeCamera();
    }
    else
    {
        DisableFreeCamera();
    }

    engineAppCommon.GetFreeCameraControl().GetRawPointer()->SetActive(isFreeCameraActive);
}

void FreeCamera::EnableFreeCamera()
{
    ZApplicationEngineWin32* applicationEngineWin32 = ZApplicationEngineWin32::GetInstance();
    ZEngineAppCommon& engineAppCommon = applicationEngineWin32->GetEngineAppCommon();

    TEntityRef<ZCameraEntity> freeCamera = engineAppCommon.GetFreeCamera();

    if (!freeCamera.GetRawPointer())
    {
        return;
    }

    TEntityRef<IRenderDestinationEntity> renderDestinationEntity = RenderManager->GetGameRenderDestinationEntity();
    TEntityRef<ZCameraEntity> playerCamera = renderDestinationEntity.GetRawPointer()->GetSource();

    engineAppCommon.SetMainCamera(playerCamera);
    engineAppCommon.CopyMainCameraSettingsToFreeCamera();
    renderDestinationEntity.GetRawPointer()->SetSource(freeCamera.GetEntityRef());

    ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

    if (hitman)
    {
        ZHM5InputControl* inputControl = hitman->GetInputControl();

        if (inputControl)
        {
            inputControl->DisableBindings();
        }
    }
}

void FreeCamera::DisableFreeCamera()
{
    ZApplicationEngineWin32* applicationEngineWin32 = ZApplicationEngineWin32::GetInstance();
    ZEngineAppCommon& engineAppCommon = applicationEngineWin32->GetEngineAppCommon();
    TEntityRef<IRenderDestinationEntity> renderDestinationEntity = RenderManager->GetGameRenderDestinationEntity();
    TEntityRef<ZCameraEntity> mainCamera;

    renderDestinationEntity.GetRawPointer()->SetSource(engineAppCommon.GetMainCamera().GetEntityRef());
    engineAppCommon.SetMainCamera(mainCamera);

    ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

    if (hitman)
    {
        ZHM5InputControl* inputControl = hitman->GetInputControl();

        if (inputControl)
        {
            inputControl->EnableBindings();
        }
    }
}

void FreeCamera::InstantlyKillNpc()
{
    ZRayQueryOutput rayQueryOutput{};

    if (GetFreeCameraRayCastClosestHitQueryOutput(ERayDetailLevel::RAYDETAILS_MESH, rayQueryOutput) && rayQueryOutput.GetBlockingEntity().GetEntityTypePtrPtr())
    {
        ZActor* actor = rayQueryOutput.GetBlockingEntity().QueryInterfacePtr<ZActor>();

        if (actor)
        {
            actor->GetID().SignalInputPin("KillActor");
        }
    }
}

void FreeCamera::TeleportMainCharacter()
{
    ZRayQueryOutput rayQueryOutput{};

    if (GetFreeCameraRayCastClosestHitQueryOutput(ERayDetailLevel::RAYDETAILS_BONES, rayQueryOutput) && rayQueryOutput.GetBlockingEntity().GetEntityTypePtrPtr())
    {
        const TEntityRef<ZHitman5>& hitman = LevelManager->GetHitman();

        if (hitman.GetRawPointer())
        {
            ZSpatialEntity* spatialEntity = hitman.GetEntityRef().QueryInterfacePtr<ZSpatialEntity>();
            SMatrix worldMatrix = spatialEntity->GetObjectToWorldMatrix();

            worldMatrix.Trans = rayQueryOutput.GetPosition();

            spatialEntity->SetObjectToWorldMatrix(worldMatrix);
        }
    }
}

bool FreeCamera::GetFreeCameraRayCastClosestHitQueryOutput(const ERayDetailLevel detailLevel, ZRayQueryOutput& rayQueryOutput)
{
    ZApplicationEngineWin32* applicationEngineWin32 = ZApplicationEngineWin32::GetInstance();
    ZEngineAppCommon& engineAppCommon = applicationEngineWin32->GetEngineAppCommon();
    TEntityRef<ZCameraEntity> freeCamera = engineAppCommon.GetFreeCamera();

    SMatrix worldMatrix = freeCamera.GetRawPointer()->GetObjectToWorldMatrix();
    float4 invertedDirection = float4(-worldMatrix.ZAxis.x, -worldMatrix.ZAxis.y, -worldMatrix.ZAxis.z, -worldMatrix.ZAxis.w);
    float4 from = worldMatrix.Trans;
    float4 to = worldMatrix.Trans + invertedDirection * 500.f;

    if (!CollisionManager)
    {
        return false;
    }

    ZRayQueryInput rayQueryInput = ZRayQueryInput(from, to, detailLevel);

    if (!CollisionManager->RayCastClosestHit(rayQueryInput, &rayQueryOutput))
    {
        return false;
    }

    return true;
}

DEFINE_MOD(FreeCamera);
