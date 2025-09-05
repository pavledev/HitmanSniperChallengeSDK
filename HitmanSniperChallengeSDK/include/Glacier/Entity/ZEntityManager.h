#pragma once

#include "../IComponentInterface.h"
#include "../Scene/IEntitySceneContext.h"
#include "../Entity/ZEntityType.h"
#include "IEntityBlueprintFactory.h"

class HitmanSniperChallengeSDK_API ZEntityManager : public IComponentInterface
{
public:
	const IEntityBlueprintFactory* GetBlueprintFromCreatedEntity(ZEntityType** pEntity) const;
	IEntityBlueprintFactory* GetBlueprintFromCreatedEntity(ZEntityType** pEntity);

private:
	IEntitySceneContext* m_pSceneContext;
	unsigned int m_nTransformParentPropertyID;
	unsigned int m_nObjectToParentID;
};
