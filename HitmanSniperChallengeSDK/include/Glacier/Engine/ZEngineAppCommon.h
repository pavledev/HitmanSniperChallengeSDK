#pragma once

#include "../Templates/TEntityRef.h"
#include "../Camera/ZFreeCameraControlEntity.h"
#include "../Camera/ZCameraEntity.h"

#include <Common.h>

class HitmanSniperChallengeSDK_API ZEngineAppCommon
{
public:
	void CreateFreeCameraAndControl();
	TEntityRef<ZFreeCameraControlEntity> GetFreeCameraControl() const;
	TEntityRef<ZCameraEntity> GetFreeCamera() const;
	TEntityRef<ZCameraEntity> GetMainCamera() const;
	void CopyMainCameraSettingsToFreeCamera();
	const bool GetIsGameInputActive() const;
	void SetIsGameInputActive(bool isGameInputActive);
	void SetFreeCamera(TEntityRef<ZCameraEntity>& freeCamera);
	void SetMainCamera(TEntityRef<ZCameraEntity>& mainCamera);

private:
	PAD(0x20);
	TEntityRef<ZFreeCameraControlEntity> m_pFreeCameraControl; //0x20
	TEntityRef<ZCameraEntity> m_pFreeCamera; //0x28
	TEntityRef<ZCameraEntity> m_pMainCamera; //0x30
	bool m_bIsGameInputActive; //0x38
};
