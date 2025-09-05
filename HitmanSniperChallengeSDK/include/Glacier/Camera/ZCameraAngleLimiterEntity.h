#pragma once

#include "../Entity/ZEntityImpl.h"

class ZCameraAngleLimiterEntity : public ZEntityImpl
{
public:
	PAD(0x4);
	bool m_bActive;
};
