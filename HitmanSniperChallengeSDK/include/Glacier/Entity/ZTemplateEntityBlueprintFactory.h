#pragma once

#include "ZCompositeEntityBlueprintFactoryBase.h"
#include "../ZBitArray.h"
#include "../Resource/ZRuntimeResourceID.h"
#include "SPinInfo.h"

class HitmanSniperChallengeSDK_API ZTemplateEntityBlueprintFactory : public ZCompositeEntityBlueprintFactoryBase
{
public:
	virtual ~ZTemplateEntityBlueprintFactory() = default;

	int GetSubEntitiesCount() const;
	ZEntityType** GetSubEntity(ZEntityType** pEntity, int index) const;
	ZEntityType* GetSubEntityType(unsigned int nSubEntity) const;
	IEntityBlueprintFactory* GetBlueprintResource(const unsigned int entityIndex) const;
	const int GetRootEntityIndex() const;
	const ZRuntimeResourceID& GetRuntimeResourceID() const;

private:
	TArray<IEntityBlueprintFactory*> m_blueprintResources;
	TArray<ZEntityType*> m_aEntityTypes;
	ZBitArray m_aModifiedEntityType;
	int m_rootEntityIndex;
	ZRuntimeResourceID m_ridResource;
};
