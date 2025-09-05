#pragma once

#include <Common.h>

class HitmanSniperChallengeSDK_API ZHM5InputControl
{
public:
	void EnableInputControl(bool bValue);
	void EnableBindings();
	void DisableBindings();

private:
	PAD(0xA0);
	bool m_bActive; //0xA0
	int m_nBindingsDisabledCount; //0xA4
};
