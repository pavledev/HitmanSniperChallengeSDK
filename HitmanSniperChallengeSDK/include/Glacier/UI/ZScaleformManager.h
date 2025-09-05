#pragma once

#include "IScaleformManager.h"

class HitmanSniperChallengeSDK_API ZScaleformManager : public IScaleformManager
{
public:
	bool IsInMainMenu() const;

private:
	PAD(0x107);
	bool m_bIsInMainMenu; //0x10B
};
