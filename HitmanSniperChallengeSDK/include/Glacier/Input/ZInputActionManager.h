#pragma once

#include <Glacier/IComponentInterface.h>
#include <Glacier/ZGameTime.h>

class ZActionMapTree;

class HitmanSniperChallengeSDK_API ZInputActionManager : public IComponentInterface
{
public:
	void SetEnabled(const bool enabled);
	bool AddBindings(const char* binds);

private:
	ZActionMapTree* m_pkRoot;
	ZActionMapTree* m_pkCurrentBlock;
	int m_iBindMem;
	bool m_bDebugKeys;
	bool m_bEnabled;
	bool m_bSpeedRepeat;
	bool m_JoinControllers;
	ZGameTime m_EventHorizonDelay;
};
