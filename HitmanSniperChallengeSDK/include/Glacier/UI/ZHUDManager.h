#pragma once

#include "../IComponentInterface.h"
#include "IScaleformExternalInterfaceHandler.h"
#include "../Templates/TFixedArray.h"
#include "EHUDItem.h"
#include "ECrossHairType.h"
#include "EHUDFadeReason.h"
#include "IScaleformPlayer.h"

class HitmanSniperChallengeSDK_API ZHUDManager : public IComponentInterface, public IScaleformExternalInterfaceHandler
{
public:
	bool IsPauseMenuActive();
	IScaleformPlayer* GetHUD() const;

private:
	PAD(0x4);
	IScaleformPlayer* m_mcHUD; //0xC
	PAD(0x68);
	bool m_bPauseMenuActive; //0x78
};
