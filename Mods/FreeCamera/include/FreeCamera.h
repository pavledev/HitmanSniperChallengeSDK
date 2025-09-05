#pragma once

#include <string>
#include <unordered_map>

#include <Glacier/ZGameLoopManager.h>
#include <Glacier/Entity/ZEntityRef.h>
#include <Glacier/Input/ZInputAction.h>
#include <Glacier/Physics/ZRayQueryOutput.h>
#include <Glacier/Physics/ERayDetailLevel.h>

#include <ModInterface.h>

class FreeCamera : public ModInterface
{
public:
    FreeCamera();
    ~FreeCamera() override;

    void OnEngineInitialized() override;
    void OnDrawMenu() override;
    void OnDrawUI(const bool hasFocus) override;

private:
    void OnFrameUpdate(const SGameUpdateEvent& updateEvent);
    void ToggleFreeCamera();
    void EnableFreeCamera();
    void DisableFreeCamera();

    void InstantlyKillNpc();
    void TeleportMainCharacter();
    bool GetFreeCameraRayCastClosestHitQueryOutput(const ERayDetailLevel detailLevel, ZRayQueryOutput& rayQueryOutput);

    bool areBindingsAdded;
    volatile bool isFreeCameraActive;
    volatile bool freezeCamera;
    ZInputAction toggleFreeCameraAction;
    ZInputAction freezeFreeCameraAction;
    ZInputAction instantlyKillNpcAction;
    ZInputAction teleportMainCharacterAction;
    bool areControlsVisible;
    std::unordered_map<std::string, std::string> pcControls;
    std::unordered_map<std::string, std::string> controllerControls;
};

DECLARE_MOD(FreeCamera)
