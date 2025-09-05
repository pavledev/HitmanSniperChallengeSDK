#pragma once

#include "ZCameraEntity.h"

class HitmanSniperChallengeSDK_API ZHM5MainCamera : public ZCameraEntity
{
public:
	void SetCameraDirection(const float4& vLookTo);

private:
	PAD(0x3C0);
};
