#include "imgui.h"

#include "Glacier/Resource/ZResourceManager.h"
#include "Glacier/Morpheme/ZAnimationResource.h"
#include "Glacier/ZGameLoopManager.h"
#include "Glacier/Module/ZHitman5Module.h"
#include "Glacier/Render/ZSpatialEntity.h"
#include "Glacier/Entity/ZTemplateEntityBlueprintFactory.h"
#include "Glacier/ZHM5CrippleBox.h"
#include "Glacier/Camera/ZCameraAngleLimiterEntity.h"
#include "Glacier/ZLevelManager.h"

#include "FreeRoam.h"
#include "Hooks.h"

FreeRoam::~FreeRoam()
{
    Hooks::ZAnimationDatabase_GetAnimSource.RemoveHook();
}

void FreeRoam::Initialize()
{
    ModInterface::Initialize();
}

void FreeRoam::OnEngineInitialized()
{
    const ZMemberDelegate<FreeRoam, void(const SGameUpdateEvent&)> delegate(this, &FreeRoam::OnFrameUpdate);

    GameLoopManager->RegisterForFrameUpdate(delegate, 1);
}

void FreeRoam::OnDrawMenu()
{
    if (ImGui::Checkbox("Enable Free Roam", &isFreeRoamEnabled))
    {
        if (isFreeRoamEnabled)
        {
            Hooks::ZAnimationDatabase_GetAnimSource.CreateHook("ZAnimationDatabase_GetAnimSource", 0x23BA90, ZAnimationDatabase_GetAnimSourceHook);
            Hooks::ZAnimationDatabase_GetAnimSource.EnableHook();
        }
    }
}

MR::IAnimSource* FreeRoam::GetAnimSource(ZAnimationDatabase* animationDatabase, const ZString& name)
{
    if (!this->animationDatabase)
    {
        ZResourcePtr chrtResourcePtr = ResourceManager->GetResourcePtr(ZRuntimeResourceID(0x800020061AD429AF), 0);
        this->animationDatabase = static_cast<ZAnimationDatabase*>(chrtResourcePtr.GetRawPointer());

        ZResourcePtr mjbaResourcePtr1 = ResourceManager->GetResourcePtr(ZRuntimeResourceID(0x800018861AD429AF), 0);
        crouchAimAnimationResource = static_cast<ZAnimationResource*>(mjbaResourcePtr1.GetRawPointer());

        ZResourcePtr mjbaResourcePtr2 = ResourceManager->GetResourcePtr(ZRuntimeResourceID(0x800018961AD429AF), 0);
        locomotionSneakIdleCorrectiveAnimationResource = static_cast<ZAnimationResource*>(mjbaResourcePtr2.GetRawPointer());

        ZResourcePtr mjbaResourcePtr3 = ResourceManager->GetResourcePtr(ZRuntimeResourceID(0x800018981AD429AF), 0);
        locomotionSneakIdlePoseAnimationResource = static_cast<ZAnimationResource*>(mjbaResourcePtr3.GetRawPointer());

        ZResourcePtr mjbaResourcePtr4 = ResourceManager->GetResourcePtr(ZRuntimeResourceID(0x800018A41AD429AF), 0);
        locomotionStandIdleCorrectiveAnimationResource = static_cast<ZAnimationResource*>(mjbaResourcePtr4.GetRawPointer());

        ZResourcePtr mjbaResourcePtr5 = ResourceManager->GetResourcePtr(ZRuntimeResourceID(0x800018A61AD429AF), 0);
        locomotionStandIdlePoseAnimationResource = static_cast<ZAnimationResource*>(mjbaResourcePtr5.GetRawPointer());

        ZResourcePtr mjbaResourcePtr6 = ResourceManager->GetResourcePtr(ZRuntimeResourceID(0x800018B41AD429AF), 0);
        reloadAnimationResource = static_cast<ZAnimationResource*>(mjbaResourcePtr6.GetRawPointer());

        ZResourcePtr mjbaResourcePtr7 = ResourceManager->GetResourcePtr(ZRuntimeResourceID(0x800018B61AD429AF), 0);
        standAimAnimationResource = static_cast<ZAnimationResource*>(mjbaResourcePtr7.GetRawPointer());
    }

    if (animationDatabase == this->animationDatabase)
    {
        std::string name2 = name.ToCString();

        if (name2 == "Crouch_Aim")
        {
            return crouchAimAnimationResource->m_pAnimationSource;
        }
        else if (name2 == "Locomotion_Sneak_Idle_Corrective")
        {
            return locomotionSneakIdleCorrectiveAnimationResource->m_pAnimationSource;
        }
        else if (name2 == "Locomotion_Sneak_Idle_Pose")
        {
            return locomotionSneakIdlePoseAnimationResource->m_pAnimationSource;
        }
        else if (name2 == "Locomotion_Stand_Idle_Corrective")
        {
            return locomotionStandIdleCorrectiveAnimationResource->m_pAnimationSource;
        }
        else if (name2 == "Locomotion_Stand_Idle_Pose")
        {
            return locomotionStandIdlePoseAnimationResource->m_pAnimationSource;
        }
        else if (name2 == "Reload")
        {
            return reloadAnimationResource->m_pAnimationSource;
        }
        else if (name2 == "Stand_Aim")
        {
            return standAimAnimationResource->m_pAnimationSource;
        }
    }

    return nullptr;
}

void FreeRoam::OnFrameUpdate(const SGameUpdateEvent& updateEvent)
{
    if (!isFreeRoamEnabled)
    {
        return;
    }

    ZEntitySceneContext* entitySceneContext = Hitman5Module->GetSceneContext();

    if (!entitySceneContext)
    {
        return;
    }

    if (entitySceneContext->m_ridSceneFactory.GetID() != 0x800003E9A5B8D926)
    {
        return;
    }

    ZSpatialEntity* spatialEntity = *reinterpret_cast<ZSpatialEntity**>(reinterpret_cast<char*>(entitySceneContext) + 0x18);

    if (!spatialEntity)
    {
        return;
    }

    ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

    if (!hitman)
    {
        return;
    }

    ZEntityRef sceneEntityRef = spatialEntity->GetID();
    ZResourcePtr resourcePtr = ResourceManager->GetResourcePtr(ZRuntimeResourceID(0x80001E041AD429AF), 0);
    ZTemplateEntityBlueprintFactory* templateEntityBlueprintFactory = static_cast<ZTemplateEntityBlueprintFactory*>(resourcePtr.GetRawPointer());

    ZEntityRef cameraAngleLimiterEntityRef = templateEntityBlueprintFactory->GetSubEntity(sceneEntityRef.GetEntityTypePtrPtr(), 3476);
    ZCameraAngleLimiterEntity* cameraAngleLimiterEntity = cameraAngleLimiterEntityRef.QueryInterfacePtr<ZCameraAngleLimiterEntity>();

    if (cameraAngleLimiterEntity->m_bActive)
    {
        cameraAngleLimiterEntityRef.SignalInputPin("Disable");

        ZEntityRef hm5CrippleBoxEntityRef = templateEntityBlueprintFactory->GetSubEntity(sceneEntityRef.GetEntityTypePtrPtr(), 3053);
        ZHM5CrippleBox* hm5CrippleBox = hm5CrippleBoxEntityRef.QueryInterfacePtr<ZHM5CrippleBox>();

        hm5CrippleBoxEntityRef.SetProperty("m_bCloseCombatFakeSwing", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bCloseCombatStandart", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bCloseCombatProps", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bCloseCombatPistolFinish", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bCloseCombatChoke", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bCoverTakedown", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bCoverScale", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bCoverToCover", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bRun", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bSneak", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bStandUp", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bFastWalk", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bLedges", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bShootingBallet", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bTakeClothes", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bLadder", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bPullVictimFromWindow", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bFiberWireKill", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bClimbWindow", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bThrowBodyOverRail", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bDumpBody", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bOperateCPDoor", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bHideInCloset", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bCloseCombat", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bDisableFuseBox", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bVentilatorShaft", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bPushVictimThroughWindowAndRail", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bKeyCardReader", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bPickupItem", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bDragBody", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bTossItem", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bCover", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bMovementAllowed", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bBlindFire", true);
        hm5CrippleBoxEntityRef.SetProperty("m_bIdleAnimationsAllowed", true);

        hm5CrippleBox->UpdateFlags();
        ZHM5CrippleBox::SetDataOnHitman(false);

        float4 worldPosition = hitman->GetSpatialEntityPtr()->GetWorldPosition();

        worldPosition.x = 29.081f;
        worldPosition.y = 26.852f;
        worldPosition.z = -0.464f;

        hitman->GetSpatialEntityPtr()->SetWorldPosition(worldPosition);

        ZEntityRef standEntityRef = templateEntityBlueprintFactory->GetSubEntity(sceneEntityRef.GetEntityTypePtrPtr(), 3482);

        standEntityRef.SignalInputPin("BlendOut");

        ZEntityRef scopeEntityRef = templateEntityBlueprintFactory->GetSubEntity(sceneEntityRef.GetEntityTypePtrPtr(), 3483);

        scopeEntityRef.SignalInputPin("BlendOut");

        ZEntityRef sniperContestEntityEntityRef = templateEntityBlueprintFactory->GetSubEntity(sceneEntityRef.GetEntityTypePtrPtr(), 2712);

        sniperContestEntityEntityRef.SignalInputPin("Stop");

        //sniperContestEntityEntityRef.SetProperty(947541284, 50000.f); // Change time
    }
}

MR::IAnimSource* __fastcall ZAnimationDatabase_GetAnimSourceHook(ZAnimationDatabase* pThis, int edx, const ZString& sName)
{
    MR::IAnimSource* animSource = GetModInstance()->GetAnimSource(pThis, sName);

    if (animSource)
    {
        return animSource;
    }

    return Hooks::ZAnimationDatabase_GetAnimSource.CallOriginalFunction(pThis, sName);
}

DEFINE_MOD(FreeRoam);
