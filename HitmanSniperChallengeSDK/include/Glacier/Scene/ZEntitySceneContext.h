#pragma once

#include "IEntitySceneContext.h"
#include "../Resource/ZRuntimeResourceID.h"
#include "../Templates/TResourcePtr.h"
#include "../Entity/ZEntityRef.h"
#include "../Templates/TEntityRef.h"
#include "SSceneInitParameters.h"
#include "ISceneEntity.h"

class HitmanSniperChallengeSDK_API ZEntitySceneContext : public IEntitySceneContext
{
public:
	PAD(0x4);
	ZRuntimeResourceID m_ridSceneFactory;
};
