#pragma once

#include <Common.h>

class HitmanSniperChallengeSDK_API ZGameTime
{
public:
    long long GetTicks() const;
    double ToSeconds() const;

private:
    long long m_nTicks;
};
