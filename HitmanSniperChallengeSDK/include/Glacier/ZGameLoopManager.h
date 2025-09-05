#pragma once

#include "ZDelegate.h"
#include "SGameUpdateEvent.h"
#include "IComponentInterface.h"

class HitmanSniperChallengeSDK_API ZGameLoopManager : public IComponentInterface
{
public:
	void RegisterForFrameUpdate(const ZDelegate<void __cdecl(SGameUpdateEvent const&)>& callback, int nPriority);
	void UnregisterForFrameUpdate(const ZDelegate<void __cdecl(SGameUpdateEvent const&)>& callback);
};
