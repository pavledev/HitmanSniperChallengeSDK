#pragma once

#include "IInputDevice.h"

class HitmanSniperChallengeSDK_API ZInputDevice : public IInputDevice
{
public:
    virtual ~ZInputDevice() = default;

    PAD(0x5C);
    bool m_active;
};
