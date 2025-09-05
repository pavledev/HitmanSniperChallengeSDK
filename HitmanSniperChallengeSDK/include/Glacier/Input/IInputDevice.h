#pragma once

#include <Common.h>

class HitmanSniperChallengeSDK_API IInputDevice
{
public:
    enum EDeviceType
    {
        eMOUSE_TYPE = 0,
        eKEYBOARD_TYPE = 1,
        eGAMECONTROL_TYPE = 2,
        eCOMBOCONTROL_TYPE = 4
    };

    virtual ~IInputDevice() = default;
};
