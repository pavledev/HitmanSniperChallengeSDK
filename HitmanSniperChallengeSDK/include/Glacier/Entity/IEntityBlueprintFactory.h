#pragma once

#include "../IComponentInterface.h"
#include "ZEntityType.h"

struct SEntityStats;

class HitmanSniperChallengeSDK_API IEntityBlueprintFactory : public IComponentInterface
{
public:
	virtual ~IEntityBlueprintFactory() = default;
	virtual void GetMemoryRequirements(unsigned int* param1, unsigned int* param2, unsigned int* param3) const = 0;
	virtual ZEntityType* GetFactoryEntityType() const = 0;
};
