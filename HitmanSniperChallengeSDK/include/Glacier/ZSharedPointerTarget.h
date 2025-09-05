#pragma once

#include "ISharedPointerTarget.h"

class HitmanSniperChallengeSDK_API ZSharedPointerTarget : public ISharedPointerTarget
{
private:
	long m_iRefCount;
};
