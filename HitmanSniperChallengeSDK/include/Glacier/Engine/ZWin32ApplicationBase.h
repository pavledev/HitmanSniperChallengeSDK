#pragma once

#include "ZWin32ApplicationStub.h"

class HitmanSniperChallengeSDK_API ZWin32ApplicationBase : public ZWin32ApplicationStub
{
public:
	virtual int Main(int param1, char** param2) = 0;
};
