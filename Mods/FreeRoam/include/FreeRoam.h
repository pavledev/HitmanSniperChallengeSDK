#pragma once

#include <ModInterface.h>

namespace MR
{
	class IAnimSource;
}

class ZAnimationDatabase;
class ZAnimationResource;
struct SGameUpdateEvent;

MR::IAnimSource* __fastcall ZAnimationDatabase_GetAnimSourceHook(ZAnimationDatabase* pThis, int edx, const ZString& sName);

class FreeRoam : public ModInterface
{
public:
	~FreeRoam() override;

	void Initialize() override;
	void OnEngineInitialized() override;
	void OnDrawMenu() override;

	MR::IAnimSource* GetAnimSource(ZAnimationDatabase* animationDatabase, const ZString& name);

private:
	void OnFrameUpdate(const SGameUpdateEvent& updateEvent);

	bool isFreeRoamEnabled = false;

	ZAnimationDatabase* animationDatabase = nullptr;
	ZAnimationResource* crouchAimAnimationResource = nullptr;
	ZAnimationResource* locomotionSneakIdleCorrectiveAnimationResource = nullptr;
	ZAnimationResource* locomotionSneakIdlePoseAnimationResource = nullptr;
	ZAnimationResource* locomotionStandIdleCorrectiveAnimationResource = nullptr;
	ZAnimationResource* locomotionStandIdlePoseAnimationResource = nullptr;
	ZAnimationResource* reloadAnimationResource = nullptr;
	ZAnimationResource* standAimAnimationResource = nullptr;
};

DECLARE_MOD(FreeRoam)
