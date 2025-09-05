#pragma once

#include "IEntityBlueprintFactory.h"

class HitmanSniperChallengeSDK_API ZEntityBlueprintFactoryBase : public IEntityBlueprintFactory
{
public:
	virtual ~ZEntityBlueprintFactoryBase() = default;

protected:
	unsigned int m_nNeededMemory;
	unsigned int m_nNeededAlignment;
	unsigned int m_nOffsetToType;
};
