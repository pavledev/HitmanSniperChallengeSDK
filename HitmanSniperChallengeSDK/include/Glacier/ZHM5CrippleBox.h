#pragma once

#include "Entity/ZEntityImpl.h"

class HitmanSniperChallengeSDK_API ZHM5CrippleBox : public ZEntityImpl
{
public:
	static void SetDataOnHitman(bool bDefaultFlags);
	void UpdateFlags();
};
