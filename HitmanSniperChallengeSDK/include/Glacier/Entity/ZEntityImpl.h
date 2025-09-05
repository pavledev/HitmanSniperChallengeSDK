#pragma once

#include "IEntity.h"
#include "ZEntityType.h"

class HitmanSniperChallengeSDK_API ZEntityImpl : public IEntity
{
public:
	virtual void Init() = 0;
	virtual void Start() = 0;

private:
	ZEntityType* m_pEntityType;
};
