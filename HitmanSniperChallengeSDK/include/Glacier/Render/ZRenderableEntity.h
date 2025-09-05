#pragma once

#include "ZSpatialEntity.h"

class HitmanSniperChallengeSDK_API ZRenderableEntity : public ZSpatialEntity
{
public:
	virtual void OnVisibleOnScreenChanged(bool bVisibleOnScreen) = 0;

private:
	PAD(0x10);
};
