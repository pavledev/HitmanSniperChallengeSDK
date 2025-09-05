#pragma once

#include "../ZHM5BaseCharacter.h"
#include "../Physics/ICharacterCollision.h"
#include "EActorDeathType.h"
#include "../ZCharacterTemplateAspect.h"
#include "EActorType.h"
#include "ZHM5CCProfile.h"

class HitmanSniperChallengeSDK_API ZActor : public ZHM5BaseCharacter
{
public:
	const ZString& GetActorName() const;

private:
	PAD(0x28);
	ZString m_sActorName; //0x238
};
