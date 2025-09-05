#pragma once

#include "ZApplicationBase.h"
#include "../Templates/TMap.h"

class HitmanSniperChallengeSDK_API ZWin32ApplicationStub : public ZApplicationBase
{
public:
	virtual void Initialize(const ZString& sDefaultSettings) = 0;
	virtual void AddApplicationSpecificOptions(IIniFile* pIniFile) = 0;
	virtual bool Startup() = 0;
	virtual void Shutdown() = 0;

	void ApplyOptionOverrides(int& argc, char** argv);
};
