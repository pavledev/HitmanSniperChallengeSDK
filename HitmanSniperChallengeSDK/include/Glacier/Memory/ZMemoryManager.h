#pragma once

#include <Common.h>

class IAllocator;

class HitmanSniperChallengeSDK_API ZMemoryManager
{
public:
	IAllocator* GetNormalAllocator() const;
	IAllocator* GetAllocator(unsigned int nID);

private:
	IAllocator* m_pNormalAllocator;
};
