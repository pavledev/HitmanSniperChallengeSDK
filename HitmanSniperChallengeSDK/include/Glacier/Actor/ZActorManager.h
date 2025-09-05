#pragma once

#include "../IComponentInterface.h"
#include "../Templates/TArrayRef.h"
#include "../Templates/TMaxArray.h"
#include "../Templates/TEntityRef.h"
#include "ZActor.h"

class HitmanSniperChallengeSDK_API ZActorManager : public IComponentInterface
{
public:
	TArrayRef<TEntityRef<ZActor>> GetAliveActors();

private:
	PAD(0x19424);
	TMaxArray<TEntityRef<ZActor>, 50> m_aliveActors; //0x19428
};
